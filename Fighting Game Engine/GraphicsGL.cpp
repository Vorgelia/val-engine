#include "GraphicsGL.h"
#include "GameInstance.h"
#include "BaseShader.h"
#include "ComputeShader.h"
#include "Texture.h"
#include "FrameBuffer.h"
#include "GraphicsBuffer.h"
#include "GraphicsBindingData.h"
#include "Font.h"
#include "Mesh.h"
#include "CachedMesh.h"
#include "Material.h"
#include "DebugLog.h"
#include "InputManager.h"
#include <memory>

VE_OBJECT_DEFINITION(GraphicsGL);

Texture GraphicsGL::CreateTexture(const std::string& name, const std::vector<unsigned char>& pixels, glm::ivec2 dimensions, int format, GLuint filt, GLuint edgeBehaviour)
{
	GLuint textureId;
	glGenTextures(1, &textureId);

	Texture texture = Texture(name, textureId, dimensions, format, filt, edgeBehaviour);

	BindTexture(texture, 0);

	int dataType = format;
	switch(format)
	{
	case GL_RGBA16F:
	case GL_RGBA32F:
	case GL_RGBA16:
	case GL_RGBA8:
		dataType = GL_RGBA;
		break;
	}

	const unsigned char* data = (!pixels.empty()) ? &pixels[0] : nullptr;
	glTexImage2D(GL_TEXTURE_2D, 0, format, dimensions.x, dimensions.y, 0, dataType, GL_UNSIGNED_BYTE, data);

	UpdateTexture(texture, pixels);
	return texture;
}

void GraphicsGL::UpdateTexture(Texture& texture, const std::vector<unsigned char>& pixels)
{
	BindTexture(texture, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, texture._edgeBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, texture._edgeBehaviour);

	if(texture._filtering == GL_NEAREST)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, texture._filtering);
}

void GraphicsGL::DestroyTexture(Texture& texture) const
{
	glDeleteTextures(1, &texture._id);
	texture._valid = false;
}

GLuint GraphicsGL::CombineShaderAttachments(const std::vector<ShaderAttachment>& attachments, ShaderProgramType type) const
{
	std::vector<GLuint> shaderAttachments;
	shaderAttachments.reserve(attachments.size());

	for(auto& attachment : attachments)
	{
		GLuint attachmentId = CreateShaderAttachment(attachment);
		if(attachmentId != 0)
		{
			shaderAttachments.push_back(attachmentId);
		}
	}

	return CreateShaderProgram(shaderAttachments, type);
}

bool GraphicsGL::ToggleFeature(GLenum feature, bool enable)
{
	if(_glFeatures.find(feature) == _glFeatures.end() || _glFeatures[feature] != enable)
	{
		_glFeatures[feature] = enable;
		if(enable)
			glEnable(feature);
		else
			glDisable(feature);

		return true;
	}
	return false;
}

bool GraphicsGL::BindFrameBufferId(GLuint id)
{
	if(_boundFramebuffer != id)
	{
		_boundFramebuffer = id;
		glBindFramebuffer(GL_FRAMEBUFFER, _boundFramebuffer);
		return true;
	}

	return false;
}

bool GraphicsGL::BindGraphicsBufferId(GLuint id, GLenum target)
{
	auto& iter = _boundBuffers.find(target);
	if(iter != _boundBuffers.end() && iter->second == id)
	{
		return false;
	}

	_boundBuffers.emplace(target, id);
	glBindBuffer(target, id);
	return true;
}

GLuint GraphicsGL::CreateShaderAttachment(const ShaderAttachment& shaderAttachment) const
{
	GLuint shader = glCreateShader(shaderAttachment.type);
	const char* codeStr = shaderAttachment.code.c_str();

	glShaderSource(shader, 1, &codeStr, nullptr);
	glCompileShader(shader);

	GLint shaderStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &shaderStatus);

	if(shaderStatus != GL_TRUE)
	{
		char shaderLog[512];
		glGetShaderInfoLog(shader, 512, nullptr, shaderLog);
		_debug->VE_LOG("Shader compilation failure:\n" + std::string(shaderLog), LogItem::Type::Warning);
		return 0;
	}
	else
	{
		return shader;
	}
}

GLuint GraphicsGL::CreateShaderProgram(const std::vector<GLuint>& shaders, ShaderProgramType type) const
{
	const GLuint shaderProgram = glCreateProgram();
	//Attach all the shaders into a shader program.
	for(unsigned int i = 0; i < shaders.size(); ++i)
	{
		glAttachShader(shaderProgram, shaders[i]);
	}

	switch(type)
	{
	case ShaderProgramType::Surface:
		//The OUTn variables are pre-bound to point to specific targets in framebuffers.
		glBindFragDataLocation(shaderProgram, 0, "OUT0");
		glBindFragDataLocation(shaderProgram, 1, "OUT1");
		glBindFragDataLocation(shaderProgram, 2, "OUT2");
		glBindFragDataLocation(shaderProgram, 3, "OUT3");
		glBindFragDataLocation(shaderProgram, 4, "OUT4");
		glBindFragDataLocation(shaderProgram, 5, "OUT5");
		glBindFragDataLocation(shaderProgram, 6, "OUT6");
		//These make sure the locations for vertex, uv and normal are always stable.
		//These are in turn used in mesh files to specify sizes for every attribute.
		glBindAttribLocation(shaderProgram, 0, "vertex");
		glBindAttribLocation(shaderProgram, 1, "uv");
		glBindAttribLocation(shaderProgram, 2, "normal");

		break;
	}

	glLinkProgram(shaderProgram);

	//Do the same for uniform blocks
	GLuint blockIndex = glGetUniformBlockIndex(shaderProgram, "TimeDataBuffer");
	if(blockIndex != GL_INVALID_INDEX)
		glUniformBlockBinding(shaderProgram, blockIndex, (GLuint)UniformBlockBindingPoints::TimeDataBuffer);

	blockIndex = glGetUniformBlockIndex(shaderProgram, "RenderingDataBuffer");
	if(blockIndex != GL_INVALID_INDEX)
		glUniformBlockBinding(shaderProgram, blockIndex, (GLuint)UniformBlockBindingPoints::RenderingDataBuffer);
	
	blockIndex = glGetProgramResourceIndex(shaderProgram, GL_SHADER_STORAGE_BLOCK, "CommonVec4Buffer");
	if(blockIndex != GL_INVALID_INDEX)
		glShaderStorageBlockBinding(shaderProgram, blockIndex, (GLuint)ShaderStorageBlockBindingPoints::CommonVec4Buffer);

	return shaderProgram;
}

void GraphicsGL::OnInit()
{
	_debug = _owningInstance->Debug();
}

void GraphicsGL::OnServiceInit()
{
	FT_Init_FreeType(&_freetypeLibrary);
}

void GraphicsGL::OnDestroyed()
{
	
}

FrameBuffer GraphicsGL::CreateFrameBuffer(glm::ivec2 size, int texAmount, bool depthStencil, GLint format, glm::vec4 clearColor, GLint filtering, GLuint clearFlags)
{
	FrameBuffer frameBuffer = FrameBuffer(size, texAmount, depthStencil, format, clearColor, filtering, clearFlags);

	frameBuffer._textures.reserve(texAmount);
	for(int i = 0; i < texAmount; ++i)
	{
		frameBuffer._textures.push_back(CreateTexture("", std::vector<unsigned char>(), size, format, filtering, GL_CLAMP_TO_EDGE));
	}

	UpdateFrameBuffer(frameBuffer);
	return frameBuffer;
}

void GraphicsGL::UpdateFrameBuffer(FrameBuffer& frameBuffer)
{
	if(frameBuffer.IsValid() && !frameBuffer.IsDirty())
	{
		return;
	}

	std::vector<GLuint> attachments;
	if(!frameBuffer.IsValid())
	{
		glGenFramebuffers(1, &frameBuffer._id);
	}

	BindFrameBufferId(frameBuffer._id);

	for(unsigned int i = 0; i < frameBuffer._textures.size(); ++i)
	{
		BindTexture(frameBuffer._textures[i], 0);

		if(!frameBuffer.IsValid())
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, frameBuffer._textures[i]._id, 0);
			attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
	}

	if((frameBuffer.flags() | FrameBufferFlags::DepthStencil) != FrameBufferFlags::None)
	{
		if(!frameBuffer.IsValid())
		{
			glGenRenderbuffers(1, &frameBuffer._depthStencil);
		}
		glBindRenderbuffer(GL_RENDERBUFFER, frameBuffer._depthStencil);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, frameBuffer._resolution.x, frameBuffer._resolution.y);
		if(!frameBuffer.IsValid())
		{
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, frameBuffer._depthStencil);
		}
	}

	if(!frameBuffer.IsValid())
	{
		glDrawBuffers(attachments.size(), &attachments[0]);
	}

	glClearColor(frameBuffer._clearColor.x, frameBuffer._clearColor.y, frameBuffer._clearColor.z, frameBuffer._clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		_debug->VE_LOG("Framebuffer generation failed.", LogItem::Type::Warning);
	}
	else
	{
		frameBuffer._flags |= FrameBufferFlags::Valid;
		frameBuffer._flags &= ~FrameBufferFlags::Dirty;
	}

	BindDefaultFrameBuffer();
}

void GraphicsGL::ClearFrameBuffer(FrameBuffer& frameBuffer)
{
	BindFrameBuffer(frameBuffer);
	glClearColor(frameBuffer._clearColor.x, frameBuffer._clearColor.y, frameBuffer._clearColor.z, frameBuffer._clearColor.w);
	glClear(frameBuffer._clearFlags);
}

void GraphicsGL::DestroyFrameBuffer(FrameBuffer& frameBuffer) const
{
	for(Texture& tex : frameBuffer.textures())
	{
		DestroyTexture(tex);
	}

	if((frameBuffer.flags() | FrameBufferFlags::DepthStencil) != FrameBufferFlags::None)
		glDeleteRenderbuffers(1, &frameBuffer._depthStencil);

	if(frameBuffer.IsValid())
		glDeleteFramebuffers(1, &frameBuffer._id);

	frameBuffer._flags &= ~FrameBufferFlags::Valid;
}

void GraphicsGL::UpdateGraphicsBuffer(const GraphicsBuffer& buffer) const
{
	const std::vector<float>& bufferData = buffer.data();

	if(buffer._dataSize != bufferData.size())
	{
		glNamedBufferData(buffer._id, sizeof(float) * bufferData.size(), &bufferData[0], GL_DYNAMIC_DRAW);
		buffer._dataSize = bufferData.size();
	}
	else
	{
		glNamedBufferSubData(buffer._id, 0, sizeof(float) * bufferData.size(), &bufferData[0]);
	}
}

Font GraphicsGL::CreateFont(std::string name)
{
	Font font = Font(name);

	FT_Face face;
	if(FT_New_Face(_freetypeLibrary, name.c_str(), 0, &face))
	{
		_debug->VE_LOG("Unable to load font: " + name, LogItem::Type::Warning);
		return font;
	}

	//Force the Y size to TEXT_SIZE, X size automatic.
	FT_Set_Pixel_Sizes(face, 0, font.textSize);

	//Load all the characters into textures, one by one.
	std::vector<unsigned char> pixels(font.atlasSize * font.atlasSize);
	glm::ivec2 cursor = glm::ivec2(0, 0);

	for(GLubyte characterIndex = 0; characterIndex < 255; ++characterIndex)
	{
		if(FT_Load_Char(face, characterIndex, FT_LOAD_RENDER) != 0)
		{
			continue;
		}

		if(cursor.x + face->glyph->bitmap.width >= font.atlasSize)
		{
			cursor.x = 0;
			cursor.y += font.textSize;
			if(cursor.y >= (int)font.atlasSize)
			{
				cursor = glm::ivec2(0, 0);
				font._atlases.push_back(CreateTexture("Atlas" + std::to_string(font._atlases.size() + 1), pixels, glm::ivec2(font.atlasSize), GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE));
				pixels.clear();
				pixels.resize(font.atlasSize * font.atlasSize);
			}
		}

		//Invert the pixels vertically.
		for(unsigned int j = 0; j < face->glyph->bitmap.rows; ++j)
		{
			for(unsigned int i = 0; i < face->glyph->bitmap.width; ++i)
			{
				int verticalPos = (font.atlasSize - (1 + cursor.y + j)) * font.atlasSize;
				int horizontalPos = (cursor.x + i);
				pixels[verticalPos + horizontalPos] = face->glyph->bitmap.buffer[j*face->glyph->bitmap.width + i];
			}
		}

		glm::vec4 params = glm::vec4((float)cursor.x / (float)font.atlasSize, (float)cursor.y / (float)font.atlasSize, (float)face->glyph->bitmap.width / (float)font.atlasSize, (float)face->glyph->bitmap.rows / (float)font.atlasSize);
		font._characters.insert(std::pair<GLubyte, FontCharacter>(characterIndex, FontCharacter(font._atlases.size(), params, glm::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top), face->glyph->advance.x >> 6)));
		cursor.x += face->glyph->bitmap.width + font.atlasPadding;

		if(characterIndex == 'H')
		{
			//Hack fraud method of finding the top bearing of the entire font by finding the top bearing of one of the tallest characters.
			font._topBearing = face->glyph->bitmap_top;
			font._height = glm::max<GLuint>(font._height, face->glyph->bitmap.rows);
		}
	}
	font._atlases.push_back(CreateTexture("Atlas" + std::to_string(font._atlases.size() + 1), pixels, glm::ivec2(font.atlasSize, font.atlasSize), GL_RED, GL_LINEAR, GL_CLAMP_TO_EDGE));

	return font;
}

void GraphicsGL::DestroyFont(Font& font) const
{
	for(auto& iter : font._atlases)
	{
		DestroyTexture(iter);
	}
}

void GraphicsGL::DestroyShader(BaseShader& shader) const
{
	if(_boundShader == shader._id)
	{
		glUseProgram(0);
	}
	glDeleteProgram(shader._id);
}

Mesh GraphicsGL::CreateMesh(const std::string& name, CachedMesh* meshData)
{
	Mesh mesh = Mesh(name, meshData);
	mesh._elementAmount = meshData->elements.size();

	glGenVertexArrays(1, &mesh._vao);
	BindMesh(mesh);

	glGenBuffers(1, &mesh._vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh._vbo);
	glBufferData(GL_ARRAY_BUFFER, meshData->verts.size() * sizeof(float), &(meshData->verts.at(0)), GL_STATIC_DRAW);

	glGenBuffers(1, &mesh._ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh._ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->elements.size() * sizeof(GLuint), &meshData->elements.at(0), GL_STATIC_DRAW);

	int totalSize = 0;
	unsigned int i = 0;
	for(i = 0; i < meshData->vertexFormat.size(); ++i)
	{
		//Get total size of every vertex
		totalSize += meshData->vertexFormat[i].length;
	}

	int stride = 0;
	for(i = 0; i < meshData->vertexFormat.size(); ++i)
	{
		glEnableVertexAttribArray((GLuint)meshData->vertexFormat[i].index);
		glVertexAttribPointer((GLuint)meshData->vertexFormat[i].index, meshData->vertexFormat[i].length, GL_FLOAT, GL_FALSE, sizeof(float)*totalSize, (void*)(sizeof(float)*stride));
		stride += meshData->vertexFormat[i].length;
	}

	return mesh;
}

void GraphicsGL::DestroyMesh(Mesh & mesh) const
{
	glDeleteBuffers(1, &mesh._vbo);
	glDeleteBuffers(1, &mesh._ebo);
	glDeleteVertexArrays(1, &mesh._vao);
}

bool GraphicsGL::BindTextureUnit(GLuint pos)
{
	if(_activeTexture > _boundTextures.size())
	{
		_debug->VE_LOG("Attempting to bind texture unit beyond hardware range. (" + std::to_string(pos) + ", max " + std::to_string(_maxTextureUnits) + ")", LogItem::Type::Error);
		return false;
	}

	if(_activeTexture != pos)
	{
		_activeTexture = pos;
		glActiveTexture(GL_TEXTURE0 + pos);
		return true;
	}
	return false;
}

bool GraphicsGL::BindShader(const BaseShader& shader)
{
	if(_boundShader != shader._id)
	{
		_boundShader = shader._id;
		glUseProgram(shader._id);
		return true;
	}

	return false;
}

bool GraphicsGL::BindMesh(const Mesh& mesh)
{
	if(_boundVAO != mesh._vao)
	{
		_boundVAO = mesh._vao;
		glBindVertexArray(_boundVAO);
		return true;
	}

	return false;
}

bool GraphicsGL::BindFrameBuffer(const FrameBuffer& frameBuffer)
{
	if(frameBuffer.IsValid())
	{
		return BindFrameBufferId(frameBuffer._id);
	}

	return false;
}

bool GraphicsGL::BindGraphicsBuffer(const GraphicsBuffer& buffer, GLenum target)
{
	return BindGraphicsBufferId(buffer._id, target);
}

bool GraphicsGL::UnbindGraphicsBuffer(GLenum target)
{
	return BindGraphicsBufferId(0, target);
}

bool GraphicsGL::BindDefaultFrameBuffer()
{
	return BindFrameBufferId(0);
}

void GraphicsGL::BindTextureToImageUnit(GLuint unit, const Texture& tex, GLenum accessType) const
{
	glBindImageTexture(unit, tex._id, 0, GL_FALSE, 0, accessType, GL_RGBA16);
}

void GraphicsGL::BindBufferToBindingPoint(GLuint unit, const GraphicsBuffer& buffer) const
{
	switch(buffer._type)
	{
	case GraphicsBufferType::ShaderStorage:
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, unit, buffer._id);
		break;
	case GraphicsBufferType::Uniform:
		glBindBufferBase(GL_UNIFORM_BUFFER, unit, buffer._id);
		break;
	}
}

void GraphicsGL::ApplyMaterial(const Material& material)
{
	ApplyMaterialProperties(material);
}

void GraphicsGL::ApplyMaterialProperties(const Material & material)
{
	if(material.HasProperty(Material::Properties::Cull))
		ToggleFeature(GL_CULL_FACE, true);
	else
		ToggleFeature(GL_CULL_FACE, false);

	if(material.HasProperty(Material::Properties::Blend))
		ToggleFeature(GL_BLEND, true);
	else
		ToggleFeature(GL_BLEND, false);

	if(material.HasProperty(Material::Properties::ZWrite))
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);

	if(material.HasProperty(Material::Properties::ZTest))
		ToggleFeature(GL_DEPTH_TEST, true);
	else
		ToggleFeature(GL_DEPTH_TEST, false);

}

void GraphicsGL::DispatchCompute(const ComputeShader& shader, unsigned int workGroupSizeX, unsigned int workGroupSizeY, unsigned int workGroupSizeZ)
{
	BindShader(shader);
	glDispatchCompute(workGroupSizeX, workGroupSizeY, workGroupSizeZ);
}

bool GraphicsGL::BindTexture(const Texture& texture)
{
	if(_boundTextures.at(_activeTexture) != texture._id)
	{
		_boundTextures.at(_activeTexture) = texture._id;
		glBindTexture(GL_TEXTURE_2D, texture._id);
		return true;
	}
	return false;
}

bool GraphicsGL::BindTexture(const Texture& texture, GLuint pos)
{
	if(_boundTextures.at(pos) != texture._id)
	{
		BindTextureUnit(pos);
		return BindTexture(texture);
	}
	return false;
}

GraphicsGL::GraphicsGL()
{
	glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &_maxTextureUnits);
	_boundTextures.resize(_maxTextureUnits, (GLuint)0);
}