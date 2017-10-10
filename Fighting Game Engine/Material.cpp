#include "Material.h"
#include "Texture.h"
#include "Shader.h"
#include "ResourceLoader.h"
#include "GLStateTrack.h"
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

void Material::ApplyProperties()
{
	if(HasProperty(Properties::Cull))
		GLState::Set(GL_CULL_FACE, true);
	else
		GLState::Set(GL_CULL_FACE, false);

	if(HasProperty(Properties::Blend))
		GLState::Set(GL_BLEND, true);
	else
		GLState::Set(GL_BLEND, false);

	if(HasProperty(Properties::ZWrite))
		glDepthMask(GL_TRUE);
	else
		glDepthMask(GL_FALSE);

	if(HasProperty(Properties::ZTest))
		GLState::Set(GL_DEPTH_TEST, true);
	else
		GLState::Set(GL_DEPTH_TEST, false);
}

bool Material::HasProperty(Material::Properties pr)
{
	return ((this->properties)&((unsigned char)pr)) != 0;
}
Material::Material(const std::string& name, Shader* shader, unsigned char properties, const std::vector<uniformFloat>& floats, const std::vector<uniformVec>& vecs, const std::vector<uniformTex>& textures)
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
Material::Material(const FS::path& path)
{
	name = path.string();
	ResourceLoader::LoadMaterial(path, &shader, &properties, &uniformFloats, &uniformTextures, &uniformVectors);
}
