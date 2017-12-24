#pragma once
#include <string>
#include <glm\glm.hpp>
#include <boost\filesystem.hpp>
#include "GLIncludes.hpp"

namespace FS = boost::filesystem;
class GraphicsGL;

class Texture
{
	friend class GraphicsGL;

private:
	std::string _name;
	FS::path _path;

	glm::vec4 _size;
	bool _valid;

	GLuint _format;
	GLuint _id;
	GLuint _filtering;
	GLuint _edgeBehaviour;

public:
	const glm::vec4& size();

	Texture(std::string name, GLuint id, glm::ivec2 dim, int format, GLuint filt, GLuint edgeBehaviour);
	~Texture() = default;
};
