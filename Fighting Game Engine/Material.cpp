#include "Material.h"
#include "Texture.h"
#include "SurfaceShader.h"
#include "FilesystemManager.h"
#include "GraphicsGL.h"

MaterialTexture::MaterialTexture(Texture* ref)
{
	this->ref = ref;
	this->params = glm::vec4(0, 0, 1, 1);
}

MaterialTexture::MaterialTexture(Texture* ref, glm::vec4 params)
{
	this->ref = ref;
	this->params = params;
}

bool Material::HasProperty(Material::Properties pr) const
{
	return (this->properties & (unsigned char)pr) != 0;
}

Material::Material(const std::string& name, SurfaceShader* shader, unsigned char properties, const std::vector<uniformFloat>& floats, const std::vector<uniformVec>& vecs, const std::vector<uniformTex>& textures)
{
	this->name = name;
	this->shader = shader;
	this->properties = properties;

	for(unsigned int i = 0; i < floats.size(); ++i)
	{
		uniformFloats[floats[i].first] = floats[i].second;
	}

	for(unsigned int i = 0; i < vecs.size(); ++i)
	{
		uniformVectors[vecs[i].first] = vecs[i].second;
	}

	for(unsigned int i = 0; i < textures.size(); ++i)
	{
		uniformTextures.insert(uniformTex(textures[i].first, textures[i].second));
	}
}

Material::Material(const std::string& name)
{
	this->name = name;
}
