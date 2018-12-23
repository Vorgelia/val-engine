#include "RenderingGL.h"
#include "GameInstance.h"
#include "ScreenManager.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "SurfaceShader.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
#include "Time.h"
#include "Material.h"
#include "ResourceManager.h"
#include "GraphicsGL.h"
#include "GraphicsBuffer.h"
#include "DebugLog.h"
#include "GraphicsBindingData.h"
#include "RenderingDataBuffer.h"
#include "TimeDataBuffer.h"
#include "Vec4Buffer.h"

#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "RenderingCommand.h"

VE_OBJECT_DEFINITION(RenderingGL);

void RenderingGL::OnInit()
{
	_graphics = _owningInstance->Graphics();
	_debug = _owningInstance->Debug();
	_resourceManager = _owningInstance->ResourceManager();
	_screen = _owningInstance->ScreenManager();

	const RenderingConfigData& renderingConfigData = _owningInstance->configData().renderingConfigData;

	//Generate the main buffer and the auxiliary buffers.
	_mainBuffer = _graphics->CreateFrameBuffer(_screen->screenSize(), renderingConfigData.frameBuferTextureAmount, true, GL_RGBA16, glm::vec4(0, 0, 0, 1));

	BindFrameBufferImages(_mainBuffer.get(), (GLuint)ImageBindingPoints::MainBufferAttachment0);
	BindFrameBufferImages(_mainBuffer.get(), (GLuint)ImageBindingPoints::AuxBuffer2Attachment0);

	//The ortho mat and the screen mat might be confusing. It's less that they set a rendering resolution and more that they map the -1 to 1 coordinates to what is specified.
	//Why do we need two different ones? Screen mat maps the screen pixels from the top-left corner(0,0) to the bottom-right(1920,1080) and it's used with UI elements.
	//The ortho mat is used for rendering objects into the world and essentially controlling what the camera is seeing. Which is why it's modifiable with the world scale, and also has its center at (0,0).

	const glm::ivec2& uiRenderingScale = renderingConfigData.uiRenderingScale;
	_uiProjectionMatrix = glm::ortho(0.0f, float(uiRenderingScale.x), float(uiRenderingScale.y), 0.0f, 0.0f, 1.0f);

	//Create graphics buffers
	_timeDataBuffer = _graphics->CreateGraphicsBuffer<TimeDataBuffer>(8, GraphicsBufferType::Uniform);
	_graphics->BindBufferToBindingPoint(GLuint(UniformBlockBindingPoints::TimeDataBuffer), *_timeDataBuffer);

	_renderingDataBuffer = _graphics->CreateGraphicsBuffer<RenderingDataBuffer>(4, GraphicsBufferType::Uniform);
	_graphics->BindBufferToBindingPoint(GLuint(UniformBlockBindingPoints::RenderingDataBuffer), *_renderingDataBuffer);

	_commonComputeVec4Buffer = _graphics->CreateGraphicsBuffer<Vec4Buffer>(4096 * 4, GraphicsBufferType::ShaderStorage);
	_commonComputeVec4Buffer->SetupData(4096 * 4);
	_graphics->UpdateGraphicsBuffer(*_commonComputeVec4Buffer);
	_graphics->BindBufferToBindingPoint(GLuint(ShaderStorageBlockBindingPoints::CommonVec4Buffer), *_commonComputeVec4Buffer);

	//Register a callback for the screen resizing
	_screen->ScreenUpdated += VE_DELEGATE_FUNC(ScreenManager::ScreenUpdateEventHandler, OnScreenResize);

	_owningInstance->updateDispatcher().BindFunction(this, UpdateFunctionTiming(UpdateGroup::Rendering, UpdateType::LastFixedGameUpdate), [this]() { RenderAllCameras(); });
}

void RenderingGL::OnDestroyed()
{
	_graphics->DestroyFrameBuffer(*_mainBuffer);
}

void RenderingGL::BeginFrame()
{
	//Reset certain rendering parameters that might have been overriden in the last frame.
	//BlendFunc controls the way alpha blending happens
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	glDepthFunc(GL_LEQUAL);

	glClearColor(0, 0, 0, 0);

	//Viewport controls the rendering size in pixels based on the actual window size.
	//We set it to the full window size here to perform no added transformation to the ones we do when rendering. Try changing _screen->size.y to _screen->size.y*0.5.
	//This will be used later to force the aspect ratio to 16/9
	glViewport(0, 0, _screen->screenSize().x, _screen->screenSize().y);

	for(auto& iter : _cameras)
	{
		if(iter.IsValid())
		{
			_graphics->ClearFrameBuffer(*iter->frameBuffer());
		}
	}

	for(auto& iter : _temporaryFrameBuffers)
	{
		_graphics->ClearFrameBuffer(*iter);
	}

	_graphics->ClearFrameBuffer(*_mainBuffer);

	_timeDataBuffer->SetupData(
		_owningInstance->timeTracker().time(),
		_owningInstance->configData().gameConfigData.fixedGameUpdateInterval);
	_renderingDataBuffer->SetupData(_screen->screenSize(), _screen->screenInvSize());

	_graphics->UpdateGraphicsBuffer(*_timeDataBuffer);
	_graphics->UpdateGraphicsBuffer(*_renderingDataBuffer);
}

void RenderingGL::EndFrame()
{
	//Render the main buffer to the default buffer.
	//Set the viewport to what was calculated for a forced 16:9 aspect ratio
	glViewport(_screen->viewportSize().x, _screen->viewportSize().y, _screen->viewportSize().z, _screen->viewportSize().w);
	//Bind the default framebuffer, clear it and draw the main buffer directly.
	_graphics->BindDefaultFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), _resourceManager->GetMesh("Meshes/Base/screenQuad.vm"), _mainBuffer.get(), _resourceManager->GetMaterial("Materials/Base/Screen_FB.vmat"));

	if(_owningInstance->configData().renderingConfigData.useSingleBuffer)
	{
		glFlush();
	}
	else
	{
		glfwSwapBuffers(_screen->window());
	}
}

FrameBuffer* RenderingGL::GetTemporaryFrameBuffer()
{
	for(auto& iter : _temporaryFrameBuffers)
	{
		if(_reservedTemporaryFrameBuffers.count(iter.get()) == 0)
		{
			_reservedTemporaryFrameBuffers.emplace(iter.get());
			return iter.get();
		}
	}

	_temporaryFrameBuffers.push_back(_graphics->CreateFrameBuffer(_screen->screenSize(), 3, true, GL_RGBA16, glm::vec4(0, 0, 0, 1)));
	_reservedTemporaryFrameBuffers.emplace(_temporaryFrameBuffers.back().get());
	return _temporaryFrameBuffers.back().get();
}

void RenderingGL::ReleaseTemporaryFrameBuffer(FrameBuffer* frameBuffer)
{
	_reservedTemporaryFrameBuffers.erase(frameBuffer);
}

void RenderingGL::RenderCamera(const BaseCamera* camera)
{
	if(!ve::IsValid(camera))
	{
		return;
	}

	std::vector<RenderingCommand> renderingCommands = camera->GatherRenderingCommands();

	std::sort(renderingCommands.begin(), renderingCommands.end()
		, [](const RenderingCommand& lhs, const RenderingCommand& rhs)
	{
		return (lhs.material->renderingOrder == rhs.material->renderingOrder)
			? (lhs.transform.GetPosition().z < rhs.transform.GetPosition().z)
			: (lhs.material->renderingOrder < rhs.material->renderingOrder);
	});

	for(auto& iter : renderingCommands)
	{
		if(iter.material == nullptr || iter.mesh == nullptr)
		{
			continue;
		}

		_graphics->BindMesh(*iter.mesh);
		_graphics->ApplyMaterial(*iter.material);

		int textureIndex = 0;
		BindMaterialUniforms(*iter.material, textureIndex);

		glm::mat4 modelMatrix = glm::mat4(iter.transform.GetMatrix());
		glm::mat4 viewMatrix = camera->GetViewMatrix();
		glm::mat4 projectionMatrix = camera->GetProjectionMatrix();

		glUniformMatrix4fv(iter.material->shader->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMatrix));
		glUniformMatrix4fv(iter.material->shader->UniformLocation("ve_matrix_view"), 1, false, glm::value_ptr(viewMatrix));
		glUniformMatrix4fv(iter.material->shader->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(projectionMatrix));
		glUniformMatrix4fv(iter.material->shader->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(projectionMatrix * viewMatrix * modelMatrix));

		glDrawElements(GL_TRIANGLES, iter.mesh->elementAmount(), GL_UNSIGNED_INT, nullptr);
	}
}

void RenderingGL::RenderAllCameras()
{
	for(auto& iter : _cameras)
	{
		if(!iter->enabled)
		{
			continue;
		}

		RenderCamera(iter.get());
	}
}

//Screen resize callback. Resize all framebuffers to match the screen size.
void RenderingGL::OnScreenResize()
{
	_mainBuffer->SetResolution(_screen->screenSize());
	_graphics->UpdateFrameBuffer(*_mainBuffer);
}

void RenderingGL::BindMaterialUniforms(const Material& material, int& inout_textureUnitOffset) const
{
	inout_textureUnitOffset = 0;
	_graphics->BindShader(*material.shader);
	//Bind uniforms specified in the material.
	for(auto iter = material.uniformFloats.begin(); iter != material.uniformFloats.end(); ++iter)
	{
		glUniform1f(material.shader->UniformLocation(iter->first), iter->second);
	}
	for(auto iter = material.uniformVectors.begin(); iter != material.uniformVectors.end(); ++iter)
	{
		glUniform4f(material.shader->UniformLocation(iter->first), iter->second.x, iter->second.y, iter->second.z, iter->second.w);
	}

	for(auto iter = material.uniformTextures.begin(); iter != material.uniformTextures.end(); ++iter)
	{
		//Only bind a texture and its other variables if it's going to be used in the shader. If it isn't, its uniform location will be -1.
		if(material.shader->UniformLocation(iter->first) > -1)
		{
			glm::vec4 texSize = iter->second.ref->size();
			glm::vec4 params = iter->second.params;

			//Bind the current texture at the specified index.
			_graphics->BindTexture(*iter->second.ref, inout_textureUnitOffset);
			glUniform1i(material.shader->UniformLocation(iter->first), inout_textureUnitOffset);

			//Params are used for cropping the texture, size passes information about the texture itself.
			glUniform4f(material.shader->UniformLocation(iter->first + ("_params")), params.x, params.y, params.z, params.w);
			glUniform4f(material.shader->UniformLocation(iter->first + ("_size")), texSize.x, texSize.y, texSize.z, texSize.w);

			inout_textureUnitOffset += 1;
		}
	}
}

void RenderingGL::BindShaderTextures(SurfaceShader* shader, const std::vector<MaterialTexture>& textures, int& inout_textureUnitOffset) const
{
	int uniformIndex = 0;
	for(auto& iter = textures.begin(); iter != textures.end(); ++iter)
	{
		if(shader->UniformLocation("tex" + std::to_string(uniformIndex)) > -1)
		{
			Texture* tex = iter->ref;
			glm::vec4 texParams = iter->params;
			glm::vec4 texSize = tex->size();

			_graphics->BindTexture(*tex, inout_textureUnitOffset);
			glUniform1i(shader->UniformLocation("tex" + std::to_string(uniformIndex)), inout_textureUnitOffset);
			glUniform4f(shader->UniformLocation("tex" + std::to_string(uniformIndex) + ("_params")), texParams.x, texParams.y, texParams.z, texParams.w);
			glUniform4f(shader->UniformLocation("tex" + std::to_string(uniformIndex) + ("_size")), texSize.x, texSize.y, texSize.z, texSize.w);

			++inout_textureUnitOffset;
			++uniformIndex;
		}
	}
}

void RenderingGL::BindBufferUniforms(SurfaceShader* shad, int& inout_textureUnitOffset)
{
	//Bind the textures of every framebuffer to the shader.
	for(unsigned int i = 0; i < _mainBuffer->_textures.size(); ++i)
	{
		if(shad->UniformLocation("mainBuf_tex" + std::to_string(i)) > -1)
		{
			_graphics->BindTexture(*(_mainBuffer->_textures[i]), inout_textureUnitOffset);
			glUniform1i(shad->UniformLocation("mainBuf_tex" + std::to_string(i)), inout_textureUnitOffset);
			inout_textureUnitOffset += 1;
		}
	}
}

void RenderingGL::BindFrameBufferImages(const FrameBuffer* buffer, GLuint bindingPoint) const
{
	if(buffer == nullptr)
	{
		return;
	}

	for(size_t i = 0; i < buffer->_textures.size(); ++i)
	{
		_graphics->BindTextureToImageUnit(bindingPoint + i, *(buffer->_textures[i]));
	}
}

//Draw a screen mesh with a framebuffer instead of textures.
void RenderingGL::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* frameBuffer, Material* mat, glm::vec4 params)
{
	std::vector<MaterialTexture> textures;
	for(auto& texture : frameBuffer->textures())
	{
		textures.emplace_back(texture, params);
	}

	DrawScreenMesh(rect, mesh, textures, mat);
}

//Draw a screen mesh with just a material. The shader would just use the texture uniforms in the material.
void RenderingGL::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, Material* mat)
{
	DrawScreenMesh(rect, mesh, std::vector<MaterialTexture>(), mat);
}

//Standard draw screen mesh call. Takes a mesh, texture indexes/size, material and transform parameters.
void RenderingGL::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, const std::vector<MaterialTexture>& textures, Material* mat)
{
	if(mesh == nullptr)
	{
		mesh = _resourceManager->GetMesh("Meshes/Base/screenQuad.vm");
	}
	_graphics->BindMesh(*mesh);

	if(mat == nullptr)
	{
		mat = _resourceManager->GetMaterial("Materials/Base/Screen.vmat");
	}

	int textureIndex = 0;
	_graphics->ApplyMaterial(*mat);
	BindMaterialUniforms(*mat, textureIndex);
	BindShaderTextures(mat->shader, textures, textureIndex);

	glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(rect.x, rect.y + rect.w, 0));
	modelMat = glm::scale(modelMat, glm::vec3(rect.z, -rect.w, 1));
	glm::mat4 mvpmat = _uiProjectionMatrix * modelMat;

	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(_uiProjectionMatrix));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));

	glDrawElements(GL_TRIANGLES, mesh->elementAmount(), GL_UNSIGNED_INT, nullptr);
}


void RenderingGL::ApplyMaterialToFrameBuffer(const FrameBuffer* frameBuffer, const Material* material)
{

}

//TODO: Refactor, use instancing
void RenderingGL::DrawScreenText(glm::vec4 rect, GLuint size, const std::string& text, Font* font, TextAlignment alignment)
{
	if(text.empty())
		return;

	if(font == nullptr)
	{
		font = _resourceManager->GetFont(_owningInstance->configData().renderingConfigData.defaultFont);
	}

	float scale = float(size) / float(font->textSize);

	if(font == nullptr)
	{
		_debug->VE_LOG("Null font passed in DrawScreenText and VE_FONT_DEFAULT could not be loaded.", LogItem::Type::Error);
		return;
	}

	glm::ivec2 cursor = glm::ivec2(rect.x, rect.y);
	int currentLine = 0;
	std::vector<float> lineIndents;

	//Pre-parse text to figure out how much to indent every line we're about to draw.
	//We do this by keeping track of a "cursor" position starting out at the top-left of each line and advancing it while doing bounds checking.
	//Of course, we don't have to indent anything if we're aligning left anyway.
	if(alignment != TextAlignment::Left)
	{
		lineIndents.reserve(3);
		for(auto ch = text.begin(); ch != text.end();)
		{
			//If the next character is a space, advance the cursor by 40% the text size
			if(*ch == ' ')
			{
				cursor.x += (int)glm::round(size*0.4);
				++ch;
				continue;
			}

			//Otherwise, get the font character.
			const FontCharacter* fc = font->GetCharacter(*ch);
			if(*ch != '\n'&&fc == nullptr)
			{
				++ch;
				continue;
			}
			//Check if it wouldn't fit in the current line, or if it's the last character, ooooor if it's a newline.
			if(*ch == '\n' || (cursor.x + fc->size.x *scale > rect.x + rect.z) || (std::next(ch) == text.end()))
			{
				//And then store how big the line is vs how big the rect is as how much to indent the line. If the text is center-aligned, only indent by half.
				++currentLine;
				lineIndents.push_back(((rect.x + rect.z) - (cursor.x + fc->size.x *scale))*((alignment == TextAlignment::Center) ? 0.5f : 1.0f));
				if(cursor.y + glm::round(font->height()*2.4) > rect.y + rect.w)
					break;
				//Move the cursor vertically and reset it horizontally.
				cursor.y += (int)glm::round(font->height()*1.4);
				cursor.x = (int)glm::round(rect.x);
			}
			else
			{
				//Otherwise, just advance the cursor by the size of the current character.
				cursor.x += (int)glm::round((fc->advance)*scale);
				++ch;
			}
		}
		currentLine = 0;
		cursor = glm::ivec2(rect.x, rect.y);
		if(alignment != TextAlignment::Left && !lineIndents.empty())
			cursor.x += (int)glm::round(lineIndents[0]);

	}

	//Bind text drawing uniforms. Just to make individual character draws a bit more efficient.
	InitTextDrawing();

	//Loop stays more or less the same, except now we draw the characters
	for(auto ch = text.begin(); ch != text.end();)
	{
		if(*ch == ' ')
		{
			cursor.x += (int)glm::round(size*0.4);
			++ch;
			continue;
		}
		const FontCharacter* fc = font->GetCharacter(*ch);
		if(*ch != '\n'&&fc == nullptr)
		{
			++ch;
			continue;
		}

		if(*ch == '\n' || (cursor.x + fc->size.x *scale > rect.x + rect.z))
		{
			++currentLine;
			//Except now we just return if the text would go below the rect bottom.
			if(cursor.y + glm::round(font->height()*2.4) > rect.y + rect.w)
				return;
			cursor.y += (int)glm::round(font->height()*1.4);
			cursor.x = (int)glm::round(rect.x);

			if(alignment != TextAlignment::Left)
				cursor.x += (int)glm::round(lineIndents[currentLine]);
		}
		else
		{
			glm::vec4 cr = glm::vec4(cursor.x + fc->bearing.x*scale, cursor.y + (font->topBearing() - fc->bearing.y)*scale, fc->size.x*scale, fc->size.y*scale);
			DrawTextCharacter(cr, fc->textureParams, font->GetAtlas(fc->atlasIndex));
			cursor.x += int(glm::round((fc->advance) * scale));
			++ch;
		}
	}
}

void RenderingGL::RegisterCamera(BaseCamera* camera)
{
	ObjectReference<BaseCamera> cameraRef{ camera };
	if(_cameras.find(cameraRef) == _cameras.end())
	{
		_cameras.emplace(cameraRef);
	}
}

void RenderingGL::UnregisterCamera(BaseCamera* camera)
{
	ObjectReference<BaseCamera> cameraRef{ camera };
	_cameras.erase(cameraRef);
}

void RenderingGL::InitTextDrawing()
{
	//Initialize states for text drawing to minimize unnecessary state changes.
	SurfaceShader* cShad = _resourceManager->GetSurfaceShader("Shaders/Base/Screen_Text");
	Mesh* cMesh = _resourceManager->GetMesh("Meshes/Base/screenQuad.vm");

	_graphics->BindMesh(*cMesh);
	_graphics->BindShader(*cShad);

	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(_uiProjectionMatrix));

	_graphics->ApplyMaterial(*_resourceManager->GetMaterial("Materials/Base/Screen.vmat"));
}

void RenderingGL::DrawTextCharacter(glm::vec4 rect, glm::vec4 params, Texture* tex) const
{
	//Draw individual text characters while assuming things set by InitTextDrawing are still bound.
	//So much more optimization could be put into this.
	SurfaceShader* cShad = _resourceManager->GetSurfaceShader("Shaders/Base/Screen_Text");
	Mesh* cMesh = _resourceManager->GetMesh("Meshes/Base/screenQuad.vm");

	_graphics->BindTexture(*tex, 0);
	glUniform1i(cShad->UniformLocation("tex0"), 0);
	glUniform4f(cShad->UniformLocation("tex0_params"), params.x, params.y, params.z, params.w);

	glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(rect.x, rect.y + rect.w, 0));
	modelMat = glm::scale(modelMat, glm::vec3(rect.z, -rect.w, 1));

	glm::mat4 mvpmat = _uiProjectionMatrix * modelMat;

	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));
	glDrawElements(GL_TRIANGLES, cMesh->elementAmount(), GL_UNSIGNED_INT, nullptr);
}