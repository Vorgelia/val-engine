#include "Texture.h"
#include "GLStateTrack.h"
#include "DebugLog.h"
bool Texture::valid()
{
	return _valid;
}
GLuint Texture::format()
{
	return _format;
}
Texture::operator const GLuint()
{
	return id;
}

Texture::Texture(std::string nm, std::vector<unsigned char> pixels, glm::ivec2 dim, int format, GLuint filt = GL_NEAREST, GLuint edgeBehaviour = GL_REPEAT, bool flip)
{
	name = nm;
	_valid = false;
	path = FS::current_path();
	filtering = filt;
	this->edgeBehaviour = edgeBehaviour;
	_format = format;

	size = glm::vec4(dim, 1.0f / (float)(dim.x), 1.0f / (float)(dim.y));

	if(flip)
	{
		std::vector<unsigned char> px;
		px.reserve(dim.x*dim.y * (format == GL_RED ? 1 : 4));
		for(int j = dim.y - 1; j >= 0; --j)
		{
			for(int i = 0; i < dim.x * (format == GL_RED ? 1 : 4); ++i)
			{
				px.push_back(pixels[j*dim.x * (format == GL_RED ? 1 : 4) + i]);
			}
		}
		Create(px);
	}
	else
		Create(pixels);
}
Texture::Texture(std::string nm, std::vector<float> pixels, glm::ivec2 dim, int format, GLuint filt = GL_NEAREST, GLuint edgeBehaviour = GL_REPEAT, bool flip)
{
	name = nm;
	_valid = false;
	path = FS::current_path();
	filtering = filt;
	this->edgeBehaviour = edgeBehaviour;
	_format = format;

	size = glm::vec4(dim, 1.0f / (float)(dim.x), 1.0f / (float)(dim.y));

	if(flip)
	{
		std::vector<float> px;
		px.reserve(dim.x*dim.y * (format == GL_RED ? 1 : 4));
		for(int j = dim.y - 1; j >= 0; --j)
		{
			for(int i = 0; i < dim.x * (format == GL_RED ? 1 : 4); ++i)
			{
				px.push_back(pixels[j*dim.x * (format == GL_RED ? 1 : 4) + i]);
			}
		}
		Create(px);
	}
	else
		Create(pixels);
}
Texture::Texture(std::string nm, FS::path path, int format, int SOILformat = SOIL_LOAD_AUTO, GLuint filt = GL_NEAREST, GLuint edgeBehaviour = GL_REPEAT)
{
	_valid = false;
	this->path = path;
	std::vector<unsigned char> px;
	glm::ivec2 dim;
	unsigned char* pixels = SOIL_load_image(path.string().c_str(), &dim.x, &dim.y, 0, SOILformat);
	if(pixels == nullptr)
	{
		name = "error";
		DebugLog::Push("Texture load error:\n\t-" + path.string() + "\n\t-" + SOIL_last_result());
	}
	else
	{
		name = nm;
		filtering = filt;
		this->edgeBehaviour = edgeBehaviour;
		_format = format;

		size = glm::vec4(dim, 1.0f / (float)(dim.x), 1.0f / (float)(dim.y));
		px.reserve(dim.x*dim.y * 4);
		for(int j = dim.y - 1; j >= 0; --j)
		{
			for(int i = 0; i < dim.x * 4; ++i)
			{
				px.push_back(pixels[j*dim.x * 4 + i]);
			}
		}
		Create(px);
	}
}



void Texture::Create(std::vector<float> px)
{
	if(px.size() == 0)
	{
		DebugLog::Push("Invalid Texture: " + name);
		return;
	}
	glGenTextures(1, &this->id);
	GLState::BindTexture(this->id, 0);
	if(_format == GL_RGBA16F || _format == GL_RGBA32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, _format, (int)size.x, (int)size.y, 0, GL_RGBA, GL_FLOAT, &px[0]);
	}
	else
		glTexImage2D(GL_TEXTURE_2D, 0, _format, (int)size.x, (int)size.y, 0, _format, GL_FLOAT, &px[0]);
	Update();
	_valid = true;

}

void Texture::Create(std::vector<unsigned char> px)
{
	if(px.size() == 0)
	{
		DebugLog::Push("Invalid Texture: " + name);
		return;
	}
	glGenTextures(1, &this->id);
	GLState::BindTexture(this->id, 0);
	if(_format == GL_RGBA16F || _format == GL_RGBA32F)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, _format, (int)size.x, (int)size.y, 0, GL_RGBA, GL_UNSIGNED_BYTE, &px[0]);
	}
	else
		glTexImage2D(GL_TEXTURE_2D, 0, _format, (int)size.x, (int)size.y, 0, _format, GL_UNSIGNED_BYTE, &px[0]);
	Update();
	_valid = true;
}
bool Texture::Bind(int ind = 0)
{
	if(_valid)
	{
		GLState::BindTexture(this->id, ind);
		return true;
	}
	return false;
}
void Texture::Update()
{
	GLState::BindTexture(this->id, 0);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, edgeBehaviour);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, edgeBehaviour);
	if(filtering == GL_NEAREST)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
}
void Texture::Destroy()
{
	glDeleteTextures(1, &id);
	GLState::BindTexture(0, 0);
	_valid = false;
}
Texture::~Texture()
{
	Destroy();
}