#pragma once
#include "CommonUtilIncludes.hpp"

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

	void Create(const std::vector<float>& px);
	void Create(const std::vector<unsigned char>& px);
	bool Bind(int ind);
	void Update();
	void Destroy();

	operator const GLuint();

	Texture(const std::string& nm, const std::vector<unsigned char>& px, glm::ivec2 dim, int format, GLuint filt, GLuint edgeBehaviour, bool flip = true);
	Texture(const std::string& nm, const std::vector<float>& px, glm::ivec2 dim, int format, GLuint filt, GLuint edgeBehaviour, bool flip = true);
	Texture(const std::string& nm, const FS::path& path, int format, int SOILformat, GLuint filt, GLuint edgeBehaviour);
	~Texture();
};
