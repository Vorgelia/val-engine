#pragma once
#include <GLM/glm.hpp>
#include <string>
#include <unordered_map>
#include <filesystem>
#include "EnumUtils.h"
#include "GLIncludes.hpp"

namespace fs = std::filesystem;

class Texture;
class SurfaceShader;
struct MaterialTexture
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

enum class RenderingOrder : std::int16_t
{
	SolidGeometry = 0,
	TransparentGeometry = 1000,
	Overlay = 2000
};

class Material
{
public:
	enum class Properties : std::uint8_t
	{
		VE_BITMASK_VALUE_NONE,
		ZWrite = 0b0001,
		ZTest = 0b0010,
		Blend = 0b0100,
		Cull = 0b1000,
		VE_BITMASK_VALUE_ALL
	};

	std::string name;
	SurfaceShader* shader;
	Properties properties;

	RenderingOrder renderingOrder;

	std::unordered_map<std::string, GLfloat> uniformFloats;
	std::unordered_map<std::string, MaterialTexture> uniformTextures;
	std::unordered_map<std::string, glm::vec4> uniformVectors;

	bool HasProperty(Properties pr) const;

	Material(const std::string& name, SurfaceShader* shader, Properties properties, const std::vector<uniformFloat>& floats, const std::vector<uniformVec>& vecs, const std::vector<uniformTex>& textures);
	Material(const std::string& name);
};

VE_DECLARE_BITMASK_ENUM(Material::Properties)