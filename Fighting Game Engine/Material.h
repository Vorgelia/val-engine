#pragma once
#include "CommonUtilIncludes.hpp"
class Texture;
class Shader;
class MaterialTexture{
public:
	Texture* ref;
	glm::vec4 params;
	MaterialTexture(Texture* ref);
	MaterialTexture(Texture* ref,glm::vec4 params);

};

typedef std::pair<std::string, float> uniformFloat;
typedef std::pair<std::string, MaterialTexture> uniformTex;
typedef std::pair<std::string, glm::vec4> uniformVec;

class Material
{
public:
	enum class Properties{
		ZWrite = 1,
		ZTest = 2,
		Blend = 4,
		Cull = 8
	};
	std::string name;
	Shader* shader;
	unsigned char properties;
	std::map<std::string, GLfloat> uniformFloats;
	std::map<std::string, MaterialTexture> uniformTextures;
	std::map<std::string, glm::vec4> uniformVectors;
	void ApplyProperties();
	inline bool HasProperty(Properties pr);
	Material(std::string name, Shader* shader, unsigned char properties, std::vector<uniformFloat> floats, std::vector<uniformVec> vecs, std::vector<uniformTex> textures);
	Material(FS::path path);
};

