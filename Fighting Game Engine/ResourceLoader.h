#pragma once
#include "CommonUtilIncludes.hpp"
class VertexAttribute;
class Shader;
class Texture;
class MaterialTexture;
class PostEffect;
class Material;
enum class InputButton;
enum class InputDirection;
class InputEvent;
namespace ResourceLoader{
	std::string LoadTextResource(int id,std::string type="TEXT");
	std::string ReturnFile(FS::path dir);
	std::vector<std::string> ReturnFileLines(FS::path dir,bool removeWhitespace);
	bool SaveFile(FS::path dir, std::string content, int flags = std::ios::out | std::ios::trunc);
	void LoadMeshVM(FS::path path, std::vector<float> **verts, std::vector<GLuint> **elements, std::vector<VertexAttribute> *vertexFormat);
	void LoadMaterial(FS::path path, Shader** shader, unsigned char* properties, std::map<std::string, GLfloat>* uniformFloats, std::map<std::string, MaterialTexture>* uniformTextures, std::map<std::string, glm::vec4>* uniformVectors);
	void LoadPostEffect(FS::path path, std::vector<std::pair<int, Material* >>* elements, bool* cbBefore, bool* cbAfter,int* order);
	void LoadControlSettings(FS::path path, std::unordered_map<InputDirection, InputEvent>* dir, std::unordered_map<InputButton, InputEvent>* bt);
}