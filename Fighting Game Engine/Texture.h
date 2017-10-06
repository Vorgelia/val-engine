#pragma once
#include "CommonUtilIncludes.hpp"

template <class T>
class CachedTexture
{
public:
	std::string name;
	FS::path path;
	std::vector<T> *pixels;
	CachedTexture(std::string name, FS::path path, std::vector<T> *pixels);
	~CachedTexture();
};

class Texture
{
private:
	bool _valid;
	GLuint _format;
public:
	std::string name;
	FS::path path;
	glm::vec4 size; //x,y,1/x,1/y
	GLuint id;
	GLuint filtering;
	GLuint edgeBehaviour;
	bool valid();
	GLuint format();

	operator const GLuint();

	Texture(const std::string& nm, const std::vector<unsigned char>& px, glm::ivec2 dim, int format, GLuint filt, GLuint edgeBehaviour, bool flip = true);
	Texture(const std::string& nm, const std::vector<float>& px, glm::ivec2 dim, int format, GLuint filt, GLuint edgeBehaviour, bool flip = true);
	Texture(const std::string& nm, const FS::path& path, int format, int SOILformat, GLuint filt, GLuint edgeBehaviour);
	void Create(const std::vector<float>& px);
	void Create(const std::vector<unsigned char>& px);
	bool Bind(int ind);
	void Update();
	void Destroy();
	~Texture();
};
