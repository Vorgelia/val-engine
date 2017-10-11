#pragma once
#include <GLM\glm.hpp>
#include <string>
#include <unordered_map>
#include <boost\filesystem.hpp>
#include "GLIncludes.hpp"

namespace FS = boost::filesystem;

class Texture;
class Shader;
class MaterialTexture
{
public:
	Texture* ref;
	glm::vec4 params;

	MaterialTexture(Texture* ref);
	MaterialTexture(Texture* ref, glm::vec4 params);
};

typedef std::pair<std::string, float> uniformFloat;
typedef std::pair<std::string, MaterialTexture> uniformTex;
typedef std::pair<std::string, glm::vec4> uniformVec;

class Material
{
public:
	enum class Properties
	{
		ZWrite = 1,
		ZTest = 2,
		Blend = 4,
		Cull = 8
	};

	std::string name;
	Shader* shader;
	unsigned char properties;
	std::unordered_map<std::string, GLfloat> uniformFloats;
	std::unordered_map<std::string, MaterialTexture> uniformTextures;
	std::unordered_map<std::string, glm::vec4> uniformVectors;

	void ApplyProperties();
	inline bool HasProperty(Properties pr);

	Material(const std::string& name, Shader* shader, unsigned char properties, const std::vector<uniformFloat>& floats, const std::vector<uniformVec>& vecs, const std::vector<uniformTex>& textures);
	Material(const FS::path& path);
};