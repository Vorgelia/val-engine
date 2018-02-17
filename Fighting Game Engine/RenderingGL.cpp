#include "RenderingGL.h"
#include "ServiceManager.h"
#include "Screen.h"
#include "FrameBuffer.h"
#include "Mesh.h"
#include "SurfaceShader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"
#include "FrameBuffer.h"
#include "Time.h"
#include "Material.h"
#include "ResourceManager.h"
#include "GraphicsGL.h"
#include "GraphicsBuffer.h"
#include "PostEffect.h"
#include "DebugLog.h"
#include "GraphicsBindingData.h"
#include "RenderingDataBuffer.h"
#include "TimeDataBuffer.h"
#include "Vec4Buffer.h"

#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>

//Define a default font.
#define VE_FONT_DEFAULT "Fonts/Amble.ttf"
//Auxiliary buffers are framebuffers used for multipass post processing effects.
#define VE_AUX_BUFFER_AMOUNT 3
//World scale controls how many world units correspond to one pixel at a resolution of 1920x1080. Object positions are integers for consistency.
#define VE_WORLD_SCALE 3

void RenderingGL::BeginFrame()
{
	//Reset certain rendering parameters that might have been overriden in the last frame.
	//BlendFunc controls the way alpha blending happens
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthMask(GL_TRUE);
	//Viewport controls the rendering size in pixels based on the actual window size.
	//We set it to the full window size here to perform no added transformation to the ones we do when rendering. Try changing _screen->size.y to _screen->size.y*0.5.
	//This will be used later to force the aspect ratio to 16/9
	glViewport(0, 0, _screen->size.x, _screen->size.y);

	tintColor = glm::vec4(1, 1, 1, 1);

	//Bind and clear all framebuffers.
	for(unsigned int i = 0; i < _auxBuffers.size(); ++i)
	{
		_graphics->ClearFrameBuffer(*_auxBuffers[i]);
	}

	_graphics->ClearFrameBuffer(*_mainBuffer);

	_timeDataBuffer->SetupData((float)_time->time, (float)_time->deltaTime, (float)_time->frameCountSinceLoad);
	_renderingDataBuffer->SetupData(_screen->size, _screen->invSize);

	_graphics->UpdateGraphicsBuffer(*_timeDataBuffer);
	_graphics->UpdateGraphicsBuffer(*_renderingDataBuffer);
}

void RenderingGL::EndFrame()
{
	//Render the main buffer to the default buffer.
	//Set the viewport to what was calculated for a forced 16:9 aspect ratio
	glViewport(_screen->viewportSize.x, _screen->viewportSize.y, _screen->viewportSize.z, _screen->viewportSize.w);
	//Bind the default framebuffer, clear it and draw the main buffer directly.
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), _resourceManager->GetMesh("Meshes/Base/screenQuad.vm"), _mainBuffer.get(), _resourceManager->GetMaterial("Materials/Base/Screen_FB.vmat"));

#ifdef VE_USE_SINGLE_BUFFER
	glFlush();
#else
	glfwSwapBuffers(_screen->window);
#endif
}

//Screen resize callback. Resize all framebuffers to match the screen size.
void RenderingGL::OnScreenResize()
{
	_mainBuffer->resolution = _screen->size;
	_graphics->UpdateFrameBuffer(*_mainBuffer);

	for(unsigned int i = 0; i < VE_AUX_BUFFER_AMOUNT; ++i)
	{
		_auxBuffers[i]->resolution = _screen->size;
		_graphics->UpdateFrameBuffer(*_auxBuffers[i]);
	}
}

void RenderingGL::BindMaterialUniforms(const Material& material, int& out_texturesBound)
{
	out_texturesBound = 0;
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
			_graphics->BindTexture(*iter->second.ref, out_texturesBound);
			glUniform1i(material.shader->UniformLocation(iter->first), out_texturesBound);

			//Params are used for cropping the texture, size passes information about the texture itself.
			glUniform4f(material.shader->UniformLocation(iter->first + ("_params")), params.x, params.y, params.z, params.w);
			glUniform4f(material.shader->UniformLocation(iter->first + ("_size")), texSize.x, texSize.y, texSize.z, texSize.w);

			out_texturesBound += 1;
		}
	}
}

void RenderingGL::BindShaderTextures(SurfaceShader* shader, const std::vector<MaterialTexture>& textures, int& out_textureUnitOffset)
{
	int uniformIndex = 0;
	for(auto& iter = textures.begin(); iter != textures.end(); ++iter)
	{
		if(shader->UniformLocation("tex" + std::to_string(uniformIndex)) > -1)
		{
			Texture* tex = iter->ref;
			glm::vec4 texParams = iter->params;
			glm::vec4 texSize = tex->size();

			_graphics->BindTexture(*tex, out_textureUnitOffset);
			glUniform1i(shader->UniformLocation("tex" + std::to_string(uniformIndex)), out_textureUnitOffset);
			glUniform4f(shader->UniformLocation("tex" + std::to_string(uniformIndex) + ("_params")), texParams.x, texParams.y, texParams.z, texParams.w);
			glUniform4f(shader->UniformLocation("tex" + std::to_string(uniformIndex) + ("_size")), texSize.x, texSize.y, texSize.z, texSize.w);

			++out_textureUnitOffset;
			++uniformIndex;
		}
	}
}

void RenderingGL::BindBufferUniforms(SurfaceShader* shad, int& index)
{
	//Bind the textures of every framebuffer to the shader.
	for(unsigned int i = 0; i < _mainBuffer->textures.size(); ++i)
	{
		if(shad->UniformLocation("mainBuf_tex" + std::to_string(i)) > -1)
		{
			_graphics->BindTexture(*(_mainBuffer->textures[i]), index);
			glUniform1i(shad->UniformLocation("mainBuf_tex" + std::to_string(i)), index);
			index += 1;
		}
	}

	for(unsigned int buf = 0; buf < _auxBuffers.size(); ++buf)
	{
		for(unsigned int i = 0; i < _auxBuffers[buf]->textures.size(); ++i)
		{
			if(shad->UniformLocation("auxBuf" + std::to_string(buf) + "_tex" + std::to_string(i)) > -1)
			{
				_graphics->BindTexture(*(_auxBuffers[buf]->textures[i]), index);
				glUniform1i(shad->UniformLocation("auxBuf" + std::to_string(buf) + "_tex" + std::to_string(i)), index);
				index += 1;
			}
		}
	}
}

void RenderingGL::BindFrameBufferImages(const FrameBuffer* buffer, GLuint bindingPoint)
{
	if(buffer == nullptr)
	{
		return;
	}

	for(size_t i = 0; i < buffer->textures.size(); ++i)
	{
		_graphics->BindTextureToImageUnit(bindingPoint + i, *(buffer->textures[i]));
	}
}

//Post effects are a good enough place to explain the entire rendering process. It doesn't change much for the rest of the functions.
void RenderingGL::DrawPostEffect(PostEffect* pf)
{
	if(pf == nullptr)
	{
		return;
	}

	//Bind the vao of the mesh we want to use for rendering. This binds its vbo and the properties that we set for it.
	Mesh* cMesh = _resourceManager->GetMesh("Meshes/Base/screenQuad.vm");
	_graphics->BindMesh(*cMesh);

	tintColor = glm::vec4(1, 1, 1, 1);

	//Each element in the element chain is an std::pair with an index and a material. It essentially renders a screen-aligned quad with that material to the framebuffer at index.
	//For each "step" specified in the current post processing effect:
	for(unsigned int pi = 0; pi < pf->elementChain.size(); ++pi)
	{
		//Wait until all graphics operations are finished.
		//Prevents IO on buffers that haven't had their effects finish rendering. Omitted for testing.
		//glFinish();

		Material* cMat = (pf->elementChain[pi].second);
		//If there is no material for this step, render the aux buffer specified to the main buffer.
		if(cMat == nullptr)
		{
			_graphics->BindFrameBuffer(*_mainBuffer);
			DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), nullptr, _auxBuffers[pf->elementChain[pi].first].get(), _resourceManager->GetMaterial("Materials/Base/Screen_FB.vmat"));
			continue;
		}

		//Index of -1 means render directly to the main buffer. Otherwise render to the indexed aux buffer.
		if(pf->elementChain[pi].first == -1)
			_graphics->BindFrameBuffer(*_mainBuffer);
		else
			_graphics->BindFrameBuffer(*(_auxBuffers[pf->elementChain[pi].first]));

		//Bind the shader in the specified material
		_graphics->BindShader(*(cMat->shader));

		//Bind the texture components of every buffer, so they can be accessible from the shader.
		int textureIndex;
		BindBufferUniforms(cMat->shader, textureIndex);

		//Generate an MVP matrix for fullscreen projection. Since our screen mat transforms clip space to 1920x1080, these values will produce a fullscreen quad.
		glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(0, 1080, 0));
		//Render the quad upside down because OpenGL is weird. We do this for all screen meshes.
		modelMat = glm::scale(modelMat, glm::vec3(1920, -1080, 1));

		glm::mat4 mvpmat = _screenMat * modelMat;

		glUniformMatrix4fv(cMat->shader->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
		glUniformMatrix4fv(cMat->shader->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(_screenMat));
		//No view matrix since this is screen-aligned and not affected by camera transformations
		//We still write the final result to mvp anyway.
		glUniformMatrix4fv(cMat->shader->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));

		//Apply material properties like ZWrite, ZTest, Blending, etc.
		if(cMat != nullptr)
			_graphics->ApplyMaterial(*cMat);

		//Finally, draw our mesh.
		glDrawElements(GL_TRIANGLES, cMesh->elementAmount(), GL_UNSIGNED_INT, 0);
	}
}

//Draw a screen mesh with a framebuffer instead of textures.
void RenderingGL::DrawScreenMesh(glm::vec4 rect, Mesh* mesh, FrameBuffer* fb, Material* mat, glm::vec4 params)
{
	std::vector<MaterialTexture> textures;
	for(unsigned int i = 0; i < fb->textures.size(); ++i)
	{
		textures.push_back(MaterialTexture(fb->textures[i].get(), params));
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
	glm::mat4 mvpmat = _screenMat*modelMat;

	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(_screenMat));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));

	glUniform4f(mat->shader->UniformLocation("ve_tintColor"), tintColor.x, tintColor.y, tintColor.z, tintColor.w);

	glDrawElements(GL_TRIANGLES, mesh->elementAmount(), GL_UNSIGNED_INT, 0);
}

void RenderingGL::DrawMesh(Transform* transform, Mesh* mesh, Material* mat, Camera* camera)
{
	if(mesh == nullptr)
	{
		_debug->VE_LOG("Attempting to draw null mesh.", LogItem::Type::Warning);
		return;
	}

	Camera* cCam = (camera == nullptr ? &(cameras[0]) : camera);

	_graphics->BindMesh(*mesh);
	_graphics->ApplyMaterial(*mat);

	int textureAmount = 0;
	BindMaterialUniforms(*mat, textureAmount);

	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(transform->ModelMatrix()));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_view"), 1, false, glm::value_ptr(cCam->ViewMatrix(transform->depth)));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(*(cCam->projectionMatrix)));
	glUniformMatrix4fv(mat->shader->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(*(cCam->projectionMatrix)*cCam->ViewMatrix(transform->depth)*transform->ModelMatrix()));

	glUniform1f(mat->shader->UniformLocation("ve_depth"), transform->depth);

	glDrawElements(GL_TRIANGLES, mesh->elementAmount(), GL_UNSIGNED_INT, 0);
}

//TODO: Refactor, use instancing
void RenderingGL::DrawScreenText(glm::vec4 rect, GLuint size, std::string text, Font* font, TextAlignment alignment)
{
	if(text.size() == 0)
		return;

	if(font == nullptr)
	{
		font = _resourceManager->GetFont(VE_FONT_DEFAULT);
	}

	float scale = (float)size / (float)font->textSize;

	//Get the specified font. If it's null, try to get the default font.
	Font* currentFont = (font == nullptr ? _resourceManager->GetFont(VE_FONT_DEFAULT) : font);

	if(currentFont == nullptr)
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
			const FontCharacter* fc = currentFont->GetCharacter(*ch);
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
				if(cursor.y + glm::round(currentFont->height()*2.4) > rect.y + rect.w)
					break;
				//Move the cursor vertically and reset it horizontally.
				cursor.y += (int)glm::round(currentFont->height()*1.4);
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
		if(alignment != TextAlignment::Left&&lineIndents.size() > 0)
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
		const FontCharacter* fc = currentFont->GetCharacter(*ch);
		if(*ch != '\n'&&fc == nullptr)
		{
			++ch;
			continue;
		}

		if(*ch == '\n' || (cursor.x + fc->size.x *scale > rect.x + rect.z))
		{
			++currentLine;
			//Except now we just return if the text would go below the rect bottom.
			if(cursor.y + glm::round(currentFont->height()*2.4) > rect.y + rect.w)
				return;
			cursor.y += (int)glm::round(currentFont->height()*1.4);
			cursor.x = (int)glm::round(rect.x);

			if(alignment != TextAlignment::Left)
				cursor.x += (int)glm::round(lineIndents[currentLine]);
		}
		else
		{
			glm::vec4 cr = glm::vec4(cursor.x + fc->bearing.x*scale, cursor.y + (currentFont->topBearing() - fc->bearing.y)*scale, fc->size.x*scale, fc->size.y*scale);
			DrawTextCharacter(cr, fc->textureParams, currentFont->GetAtlas(fc->atlasIndex));
			cursor.x += (int)glm::round((fc->advance)*scale);
			++ch;
		}
	}
}

const FrameBuffer* RenderingGL::GetFramebuffer(int index)
{
	return index < 0 ? _mainBuffer.get() : _auxBuffers[index].get();
}

void RenderingGL::Init()
{
	_graphics = _serviceManager->Graphics();
	_debug = _serviceManager->Debug();
	_resourceManager = _serviceManager->ResourceManager();
	_screen = _serviceManager->Screen();
	_time = _serviceManager->Time();

	//Generate the main buffer and the auxiliary buffers.
	_mainBuffer = _graphics->CreateFrameBuffer(_screen->size, 3, true, GL_RGBA16, glm::vec4(0, 0, 0, 1));

	_auxBuffers.reserve(VE_AUX_BUFFER_AMOUNT);
	for(unsigned int i = 0; i < VE_AUX_BUFFER_AMOUNT; ++i)
	{
		_auxBuffers.push_back(_graphics->CreateFrameBuffer(_screen->size, 3, false, GL_RGBA16F));
	}

	BindFrameBufferImages(_mainBuffer.get(), (GLuint)ImageBindingPoints::MainBufferAttachment0);
	BindFrameBufferImages(_mainBuffer.get(), (GLuint)ImageBindingPoints::AuxBuffer2Attachment0);

	glDepthFunc(GL_LEQUAL);

	//The ortho mat and the screen mat might be confusing. It's less that they set a rendering resolution and more that they map the -1 to 1 coordinates to what is specified.
	//Why do we need two different ones? Screen mat maps the screen pixels from the top-left corner(0,0) to the bottom-right(1920,1080) and it's used with UI elements.
	//The ortho mat is used for rendering objects into the world and essentially controlling what the camera is seeing. Which is why it's modifiable with the world scale, and also has its center at (0,0).
	_orthoMat = glm::ortho(-960.0*VE_WORLD_SCALE, 960.0*VE_WORLD_SCALE, 0.0, 1080.0*VE_WORLD_SCALE, 0.0, 1.0);
	_screenMat = glm::ortho(0.0, 1920.0, 1080.0, 0.0, 0.0, 1.0);
	cameras.push_back(Camera(glm::vec2(0, 0), &_orthoMat));
	cameras.back().zoomLevel = 1.25;

	//Create graphics buffers
	_timeDataBuffer = _graphics->CreateGraphicsBuffer<TimeDataBuffer>(8, GraphicsBufferType::Uniform);
	_graphics->BindBufferToBindingPoint((GLuint)UniformBufferBindingPoints::TimeDataBuffer, *_timeDataBuffer);

	_renderingDataBuffer = _graphics->CreateGraphicsBuffer<RenderingDataBuffer>(4, GraphicsBufferType::Uniform);
	_graphics->BindBufferToBindingPoint((GLuint)UniformBufferBindingPoints::RenderingDataBuffer, *_renderingDataBuffer);

	_commonComputeVec4Buffer = _graphics->CreateGraphicsBuffer<Vec4Buffer>(1024 * 4, GraphicsBufferType::ShaderStorage);
	_commonComputeVec4Buffer->SetupData(4096, 3.5f);
	_graphics->UpdateGraphicsBuffer(*_commonComputeVec4Buffer);
	_graphics->BindBufferToBindingPoint((GLuint)ShaderStorageBufferBindingPoints::CommonVec4Buffer1024, *_renderingDataBuffer);

	//Register a callback for the screen resizing
	_screen->ScreenUpdated += Screen::ScreenUpdateEventHandler::func_t([this]() { OnScreenResize(); });
}

void RenderingGL::Update() {}

void RenderingGL::InitTextDrawing()
{
	//Initialize states for text drawing to minimize unnecessary state changes.
	SurfaceShader* cShad = _resourceManager->GetShader("Shaders/Base/Screen_Text");
	Mesh* cMesh = _resourceManager->GetMesh("Meshes/Base/screenQuad.vm");

	_graphics->BindMesh(*cMesh);
	_graphics->BindShader(*cShad);

	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_projection"), 1, false, glm::value_ptr(_screenMat));

	_graphics->ApplyMaterial(*_resourceManager->GetMaterial("Materials/Base/Screen.vmat"));
}

void RenderingGL::DrawTextCharacter(glm::vec4 rect, glm::vec4 params, Texture* tex)
{
	//Draw individual text characters while assuming things set by InitTextDrawing are still bound.
	//So much more optimization could be put into this.
	SurfaceShader* cShad = _resourceManager->GetShader("Shaders/Base/Screen_Text");
	Mesh* cMesh = _resourceManager->GetMesh("Meshes/Base/screenQuad.vm");

	_graphics->BindTexture(*tex, 0);
	glUniform1i(cShad->UniformLocation("tex0"), 0);
	glUniform4f(cShad->UniformLocation("tex0_params"), params.x, params.y, params.z, params.w);

	glm::mat4 modelMat = glm::translate(glm::mat4(), glm::vec3(rect.x, rect.y + rect.w, 0));
	modelMat = glm::scale(modelMat, glm::vec3(rect.z, -rect.w, 1));

	glm::mat4 mvpmat = _screenMat*modelMat;

	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_model"), 1, false, glm::value_ptr(modelMat));
	glUniformMatrix4fv(cShad->UniformLocation("ve_matrix_mvp"), 1, false, glm::value_ptr(mvpmat));
	glDrawElements(GL_TRIANGLES, cMesh->elementAmount(), GL_UNSIGNED_INT, 0);
}

RenderingGL::RenderingGL(ServiceManager* serviceManager) : BaseService(serviceManager)
{

}

RenderingGL::~RenderingGL()
{
	_graphics->DestroyFrameBuffer(*_mainBuffer);

	for(unsigned int i = 0; i < VE_AUX_BUFFER_AMOUNT; ++i)
	{
		_graphics->DestroyFrameBuffer(*_auxBuffers[i]);
	}
}