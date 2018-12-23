#pragma once
#include <string>
#include <glm/glm.hpp>
#include <filesystem>
#include "GLIncludes.hpp"

namespace fs = std::filesystem;

class Texture
{
	friend class GraphicsGL;

private:
	std::string _name;
	fs::path _path;

	glm::vec4 _size;
	bool _valid;

	GLuint _format;
	GLuint _id;
	GLuint _filtering;
	GLuint _edgeBehaviour;

public:
	const glm::vec4& size() const;

	Texture(std::string name, GLuint id, glm::ivec2 dimensions, int format, GLuint filtering, GLuint edgeBehaviour);
	~Texture() = default;
};
