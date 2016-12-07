#include "ResourceLoader.h"
#include "Resource.h"
#include "CachedMesh.h"
#include "Material.h"
#include "Shader.h"
#include "Texture.h"
#include "PostEffect.h"
#include "CommonUtilIncludes.hpp"
#include "InputManager.h"
#include "InputDevice.h"
#include "InputEvent.h"
#include "InputFrame.h"
//Probably my least favourite part of making this engine is importing files.
//The code always looks like a mess but at least i don't have to touch it after making it.
std::string LoadTextResource(int id){//Copypasta text resource loading. Could probably be optimized.
	HRSRC hResource = FindResource(NULL,MAKEINTRESOURCE(id),(LPCSTR)"TEXT");
	if (hResource == 0){
		std::cout << "Error finding resource " << id << std::endl;
		return "ERROR";
	}
	HGLOBAL hLoadedResource = LoadResource(NULL, hResource);
	if (hLoadedResource == 0){
		std::cout << "Error loading resource " << id << std::endl;
		return "ERROR";
	}
	LPVOID pdata = LockResource(hLoadedResource);
	LPBYTE sData = (LPBYTE)pdata;
	LPTSTR sText = (LPTSTR)sData;
	int size = SizeofResource(NULL, hResource) / sizeof(char);
	return std::string(sText).substr(0, size);
}
std::string ResourceLoader::ReturnFile(FS::path dir){
	if (!FS::exists(dir))
		return "ERROR";
	std::ifstream ifs(dir.c_str());
	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	ifs.close();
	return content;
}

bool ResourceLoader::SaveFile(FS::path dir,std::string content,int flags){
	std::ofstream ofs(dir.c_str(), flags);
	if (!ofs.is_open()){
		std::cout << "Failed to write to file: " << dir.c_str() << std::endl;
		return false;
	}
	ofs << content;
	ofs.close();
	return true;
}

std::vector<std::string> ResourceLoader::ReturnFileLines(FS::path dir, bool removeWhitespace = false){
	std::cout << "--Resource: Loading File" << dir << std::endl;
	if (!FS::exists(dir))
		return{ "ERROR" };
	std::ifstream ifs(dir.c_str());
	if (!ifs.is_open()){
		std::cout << "Error opening file: " + dir.string()<<std::endl;
		return{ "ERROR" };
	}
	std::string content((std::istreambuf_iterator<char>(ifs)),
		std::istreambuf_iterator<char>());
	ifs.close();
	if (removeWhitespace){
		boost::algorithm::erase_all(content, " ");
		boost::algorithm::erase_all(content, "\t");
	}
	std::vector<std::string> lines;
	boost::split(lines, content, boost::is_any_of("\n"), boost::token_compress_on);
	return lines;
}
//I lied. Parsing files that i wrote to be simple to parse is fun.
//For some reason i'm using pointers to components instead of pointers to objects. Don't ask me why, but i stuck with it.
void ResourceLoader::LoadControlSettings(FS::path path, std::unordered_map<InputDirection, InputEvent>* dir, std::unordered_map<InputButton,InputEvent>* bt){
	std::vector<std::string> lines;
	
	if (!FS::exists(path)){
		lines = ReturnFileLines("Settings/Input/Default.vi");
		FS::copy_file("Settings/Input/Default.vi",path);
	}
	else
		lines = ReturnFileLines(path);
	if (lines.size() == 0 || (lines.size() == 1 && lines[0] == "ERROR")){
		std::cout << "Unable to load input file.\n";
		return;
	}
	dir->clear();
	bt->clear();
	int readState = 0;//0 Buttons, 1 Axes
	for (unsigned int i = 0; i < lines.size(); ++i){
		if (lines[i]==""||lines[i].substr(0, 2) == "//")
			continue;
		else if (lines[i][0] == '#'){
			if (lines[i] == "#BUTTONS")
				readState = 0;
			if (lines[i] == "#AXES")
				readState = 1;
		}
		else{
			std::vector<std::string> spl;
			std::vector<std::string> spl2;
			boost::split(spl, lines[i], boost::is_any_of(":"), boost::token_compress_on);
			boost::split(spl2, spl[1], boost::is_any_of(","), boost::token_compress_on);
			switch (readState){
			case 0:
				bt->insert(std::pair<InputButton, InputEvent>((InputButton)boost::lexical_cast<int>(spl[0]), InputEvent(boost::lexical_cast<int>(spl2[1]), spl2[0] == "t", boost::lexical_cast<int>(spl2[2]), boost::lexical_cast<float>(spl2[3]))));
				break;
			case 1:
				dir->insert(std::pair<InputDirection, InputEvent>((InputDirection)boost::lexical_cast<int>(spl[0]), InputEvent(boost::lexical_cast<int>(spl2[1]), spl2[0] == "t", boost::lexical_cast<int>(spl2[2]), boost::lexical_cast<float>(spl2[3]))));
				break;
			}
		}
	}
}
void ResourceLoader::LoadPostEffect(FS::path path, std::vector<std::pair<int, Material*>>* elements, bool* cbBefore, bool* cbAfter,int* order) {
	std::vector<std::string> lines = ReturnFileLines(path, true);
	if (lines.size() == 0 || (lines.size() == 1 && lines[0] == "ERROR")){
		std::cout << "Unable to load post effect.\n";
		return;
	}
	elements->clear();
	int readState = 0;//0 Properties, 1 Rendering Stages
	for (unsigned int i = 0; i < lines.size(); ++i){
		if (lines[i] == "" || lines[i].substr(0, 2) == "//")
			continue;
		else if (lines[i][0] == '#'){
			if (lines[i] == "#PROPERTIES")
				readState = 0;
			if (lines[i] == "#STAGES")
				readState = 1;
		}
		else{
			std::vector<std::string> spl;
			switch (readState){
			case 0:
				boost::split(spl, lines[i], boost::is_any_of("="), boost::token_compress_on);
				if (spl.size() < 2)
					continue;
				if (spl[0] == "cleanBufferBefore")
					(*cbBefore) = (spl[1] == "true");
				else if (spl[0] == "cleanBufferAfter")
					(*cbAfter) = (spl[1] == "true");
				else if (spl[0] == "order")
					(*cbAfter) = (spl[1] == "true");
				break;
			case 1:
				boost::split(spl, lines[i], boost::is_any_of(":"), boost::token_compress_on);
				if (spl.size() < 2)
					continue;
				if (spl[1] == "null")
					elements->push_back(std::pair<int, Material*>(boost::lexical_cast<int>(spl[0]), nullptr));
				else
					elements->push_back(std::pair<int, Material*>(boost::lexical_cast<int>(spl[0]), Resource::GetMaterial(spl[1])));
				break;
			}
		}
	}
}

void ResourceLoader::LoadMaterial(FS::path path, Shader** shader, unsigned char* properties, std::map<std::string, GLfloat>* uniformFloats,std::map<std::string, MaterialTexture>* uniformTextures,std::map<std::string, glm::vec4>* uniformVectors){
	std::vector<std::string> lines = ReturnFileLines(path, true);
	if (lines.size() == 0 || (lines.size() == 1 && lines[0] == "ERROR")){
		std::cout << "Unable to load material.\n";
		return;
	}
	int readState = 0;//0 Properties, 1 Uniform Floats, 2 Uniform Vectors, 3 Uniform Textures
	uniformFloats->clear();
	uniformTextures->clear();
	uniformVectors->clear();
	std::string lastTexture;
	for (unsigned int i = 0; i < lines.size(); ++i){
		if (lines[i] == "" || lines[i].substr(0, 2) == "//")
			continue;
		else if (lines[i][0] == '#'){
			if (lines[i][1] == 'P')
				readState = 0;
			else if (lines[i]=="#UNIFORM_FLOAT")
				readState = 1;
			else if (lines[i] == "#UNIFORM_VEC4")
				readState = 2;
			else if (lines[i] == "#UNIFORM_TEX")
				readState = 3;
		}
		else{
			std::vector<std::string> spl;
			boost::split(spl, lines[i], boost::is_any_of("="), boost::token_compress_on);
			std::vector<std::string> spl2;
			if (spl.size() < 2)
				continue;
			switch (readState){
			case 0:
				if (spl[0] == "properties")
					(*properties) = boost::lexical_cast<GLuint>(spl[1]);
				else if (spl[0] == "shader")
					(*shader) = Resource::GetShader(spl[1]);
				break;
			case 1:
				uniformFloats->insert(std::pair<std::string, GLfloat>(spl[0], boost::lexical_cast<GLfloat>(spl[1])));
				break;
			case 2:
				boost::split(spl2, spl[1], boost::is_any_of(","), boost::token_compress_on);
				uniformVectors->insert(std::pair<std::string, glm::vec4>(spl[0], 
					glm::vec4(
						boost::lexical_cast<GLfloat>(spl2[0]),
						boost::lexical_cast<GLfloat>(spl2[1]),
						boost::lexical_cast<GLfloat>(spl2[2]),
						boost::lexical_cast<GLfloat>(spl2[3])
					)
				));
				break;
			case 3:
				if (spl[0] == ".properties"&&lastTexture != ""){
					boost::split(spl2, spl[1], boost::is_any_of(","), boost::token_compress_on);
					if (spl2.size() < 4)
						continue;
					uniformTextures->at(lastTexture).params=glm::vec4(
						boost::lexical_cast<float>(spl2[0]),
						boost::lexical_cast<float>(spl2[1]),
						boost::lexical_cast<float>(spl2[2]),
						boost::lexical_cast<float>(spl2[3])
						);
				}
				else{
					uniformTextures->insert(std::pair<std::string,MaterialTexture>(spl[0],MaterialTexture(Resource::GetTexture(spl[1]))));
					lastTexture = spl[0];
				}
				break;
			}
		}
	}
}

void ResourceLoader::LoadMeshVM(FS::path path, std::vector<float> **verts, std::vector<GLuint> **elements, std::vector<VertexAttribute> *vertexFormat){
	
	std::vector<std::string> lines = ReturnFileLines(path, true);
	if (lines.size() == 0||(lines.size() == 1 && lines[0] == "ERROR")){
		std::cout << "	Mesh left uninitialized.\n";
		return;
	}
	int readState = 0; //0 Attribs, 1 Vertices, 2 Elements
	*verts = new std::vector<float>();
	*elements = new std::vector<GLuint>();
	//And here is my very loose implementation of a very loosely defined format. I present to you, ValMesh!
	//Look at Meshes/quad.vm as an example of how it works. It's really simple.
	//readState keeps track of what is currently being read, options being Attribs, Vertices and Elements.
	for (unsigned int i = 0; i < lines.size(); ++i){
		if (lines[i] == "" || lines[i].substr(0, 2) == "//")
			continue;
		else if (lines[i][0] == '#'){//I'm cheating here and only checking for the first character after #
			if (lines[i][1] == 'A')
				readState = 0;
			else if (lines[i][1] == 'V')
				readState = 1;
			else if (lines[i][1] == 'E')
				readState = 2;
		}
		else{
			std::vector<std::string> split;
			boost::split(split, lines[i], boost::is_any_of(","), boost::token_compress_on);//Split line by comma
			switch (readState){//Indiscriminately add stuff to the arrays based on readState
			case 0:
				for (unsigned int j = 0; j < split.size(); ++j)//Cheating yet again. Not splitting VertexAttributes by - and just taking the first and third characters.
					vertexFormat->push_back(VertexAttribute((VertexAttributeLocation)(boost::lexical_cast<int>(split[j][0])), boost::lexical_cast<int>(split[j][2])));
				break;//I bet your favourite format doesn't omit error checking
			case 1:
				for (unsigned int j = 0; j < split.size(); ++j)
					(*verts)->push_back(boost::lexical_cast<float>(split[j]));
				break;
			case 2:
				for (unsigned int j = 0; j < split.size(); ++j)
					(*elements)->push_back(boost::lexical_cast<GLuint>(split[j]));
				break;
			}
		}

	}
	
	//(*verts)->push_back();
	//(*elements)->push_back();
}
