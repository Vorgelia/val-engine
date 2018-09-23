#include "JSON.h"

#include <Windows.h>
#include <SOIL/src/SOIL.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/algorithm.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>

#include "ServiceManager.h"
#include "FilesystemManager.h"
#include "CachedMesh.h"
#include "Material.h"
#include "PostEffect.h"
#include "Transform.h"
#include "GLIncludes.hpp"
#include "InputDevice.h"
#include "InputEvent.h"
#include "DebugLog.h"
#include "ResourceManager.h"

//Probably my least favourite part of making this engine is importing files.
//The code always looks like a mess but at least i don't have to touch it after making it.
std::string FilesystemManager::LoadTextResource(int id, const std::string& type) const
{
	//Copypasta text resource loading. Could probably be optimized.
	HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(id), (LPCSTR)type.c_str());
	if(hResource == nullptr)
	{
		throw std::runtime_error("Unable to find resource with ID " + std::to_string(id));
	}

	HGLOBAL hLoadedResource = LoadResource(nullptr, hResource);
	if(hLoadedResource == nullptr)
	{
		throw std::runtime_error("Unable to load resource with ID " + std::to_string(id));
	}

	LPVOID pdata = LockResource(hLoadedResource);
	LPBYTE sData = (LPBYTE)pdata;
	LPTSTR sText = (LPTSTR)sData;

	int size = SizeofResource(nullptr, hResource) / sizeof(char);
	return std::string(sText).substr(0, size);
}

std::vector<unsigned char> FilesystemManager::LoadBinaryResource(int id, const std::string& type) const
{
	//Doesn't work, completely unused.
	HRSRC hResource = FindResource(nullptr, MAKEINTRESOURCE(id), (LPCSTR)type.c_str());
	if(hResource == nullptr)
	{
		throw std::runtime_error("Unable to find binary resource with ID " + std::to_string(id));
	}

	HGLOBAL hLoadedResource = LoadResource(nullptr, hResource);
	if(hLoadedResource == nullptr)
	{
		throw std::runtime_error("Unable to load binary resource with ID " + std::to_string(id));
	}

	LPVOID pdata = LockResource(hLoadedResource);
	LPBYTE sData = (LPBYTE)pdata;

	int size = SizeofResource(nullptr, hResource) / sizeof(unsigned char);
	return std::vector<unsigned char>((unsigned char*)sData, (unsigned char*)sData[size]);
}

std::string FilesystemManager::ReturnFile(const FS::path& dir) const
{
	//Abstraction for turning a file into a string.
	if(!FS::exists(dir))
	{
		_debug->VE_LOG("Unable to find resource at " + dir.string(), LogItem::Type::Error);
		return std::string();
	}

	std::ifstream ifs(dir.c_str());
	if(!ifs.is_open())
	{
		_debug->VE_LOG("Unable to read resource at " + dir.string(), LogItem::Type::Error);
		return std::string();
	}

	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	ifs.close();

	return content;
}

//Abstraction for turning a file into an array of its lines. Heavily used in parsing.
std::vector<std::string> FilesystemManager::ReturnFileLines(const FS::path& dir, bool removeWhitespace) const
{
	std::string& content = ReturnFile(dir);

	if(removeWhitespace)
	{
		boost::erase_all(content, " ");
		boost::erase_all(content, "\t");
	}
	if(content == "")
	{
		return std::vector<std::string>();
	}

	std::vector<std::string> lines;
	boost::split(lines, content, boost::is_any_of(std::string("\n")), boost::token_compress_on);
	return lines;
}

template<>
std::unique_ptr<json> FilesystemManager::LoadFileResource(const FS::path& path)
{
	if(!FS::exists(path))
	{
		return std::unique_ptr<json>(nullptr);
	}

	std::string& file = ReturnFile(path);

	std::unique_ptr<json> j;
	try
	{
		j = std::make_unique<json>(json::parse(file));
	}
	catch(std::invalid_argument& err)
	{
		_debug->VE_LOG("Error when parsing JSON file " + path.string() + "\n\t" + err.what(), LogItem::Type::Error);
		j->clear();
	}
	catch(...)
	{
		_debug->VE_LOG("Unhandled exception when loading JSON file " + path.string(), LogItem::Type::Error);
		j->clear();
	}

	return std::move(j);
}

template<>
std::unique_ptr<CachedMesh> FilesystemManager::LoadFileResource(const FS::path& path)
{
	if(!FS::exists(path))
	{
		return std::unique_ptr<CachedMesh>(nullptr);
	}

	std::vector<std::string> lines = ReturnFileLines(path, true);

	std::unique_ptr<CachedMesh> mesh = std::make_unique<CachedMesh>(path.string());

	int readState = 0; //0 Attribs, 1 Vertices, 2 Elements

					   //And here is my very loose implementation of a very loosely defined format. I present to you, ValMesh!
					   //Look at Meshes/quad.vm as an example of how it works. It's really simple.
					   //readState keeps track of what is currently being read, options being Attribs, Vertices and Elements.
	for(unsigned int i = 0; i < lines.size(); ++i)
	{
		if(lines[i].size() < 2 || lines[i].substr(0, 2) == "//")
		{
			continue;
		}
		else if(lines[i][0] == '#')
		{
			//I'm cheating here and only checking for the first character after #
			if(lines[i][1] == 'A')
				readState = 0;
			else if(lines[i][1] == 'V')
				readState = 1;
			else if(lines[i][1] == 'E')
				readState = 2;
		}
		else
		{
			std::vector<std::string> split;
			boost::split(split, lines[i], boost::is_any_of(","), boost::token_compress_on);//Split line by comma
			switch(readState)
			{
				//Indiscriminately add stuff to the arrays based on readState
			case 0:
				for(unsigned int j = 0; j < split.size(); ++j)//Cheating yet again. Not splitting VertexAttributes by - and just taking the first and third characters.
					mesh->vertexFormat.push_back(VertexAttribute((VertexAttributeLocation)(boost::lexical_cast<int>(split[j][0])), boost::lexical_cast<int>(split[j][2])));
				break;//I bet your favourite format doesn't omit error checking
			case 1:
				for(unsigned int j = 0; j < split.size(); ++j)
					mesh->verts.push_back(boost::lexical_cast<float>(split[j]));
				break;
			case 2:
				for(unsigned int j = 0; j < split.size(); ++j)
					mesh->elements.push_back(boost::lexical_cast<GLuint>(split[j]));
				break;
			}
		}
	}

	return mesh;
}

template<>
std::unique_ptr<Material> FilesystemManager::LoadFileResource(const FS::path& path)
{
	if(!FS::exists(path))
	{
		return std::unique_ptr<Material>(nullptr);
	}

	std::vector<std::string> lines = ReturnFileLines(path, true);

	std::unique_ptr<Material> mat = std::make_unique<Material>(path.string());

	int readState = 0;//0 Properties, 1 Uniform Floats, 2 Uniform Vectors, 3 Uniform Textures

	std::string lastTexture;

	for(unsigned int i = 0; i < lines.size(); ++i)
	{
		if(lines[i] == "" || lines[i].substr(0, 2) == "//")
			continue;
		else if(lines[i][0] == '#')
		{
			if(lines[i][1] == 'P')
				readState = 0;
			else if(lines[i] == "#UNIFORM_FLOAT")
				readState = 1;
			else if(lines[i] == "#UNIFORM_VEC4")
				readState = 2;
			else if(lines[i] == "#UNIFORM_TEX")
				readState = 3;
		}
		else
		{
			std::vector<std::string> spl;
			boost::split(spl, lines[i], boost::is_any_of("="), boost::token_compress_on);
			std::vector<std::string> spl2;
			if(spl.size() < 2)
				continue;
			switch(readState)
			{
			case 0:
				if(spl[0] == "properties")
					mat->properties = boost::lexical_cast<GLuint>(spl[1]);
				else if(spl[0] == "shader")
					mat->shader = _resource->GetSurfaceShader(spl[1]);
				break;
			case 1:
				mat->uniformFloats.insert(std::pair<std::string, GLfloat>(spl[0], boost::lexical_cast<GLfloat>(spl[1])));
				break;
			case 2:
				boost::split(spl2, spl[1], boost::is_any_of(","), boost::token_compress_on);
				mat->uniformVectors.insert(std::pair<std::string, glm::vec4>(spl[0],
					glm::vec4(
						boost::lexical_cast<GLfloat>(spl2[0]),
						boost::lexical_cast<GLfloat>(spl2[1]),
						boost::lexical_cast<GLfloat>(spl2[2]),
						boost::lexical_cast<GLfloat>(spl2[3])
					)
					));
				break;
			case 3:
				if(spl[0] == ".properties"&&lastTexture != "")
				{
					boost::split(spl2, spl[1], boost::is_any_of(","), boost::token_compress_on);
					if(spl2.size() < 4)
						continue;
					mat->uniformTextures.at(lastTexture).params = glm::vec4(
						boost::lexical_cast<float>(spl2[0]),
						boost::lexical_cast<float>(spl2[1]),
						boost::lexical_cast<float>(spl2[2]),
						boost::lexical_cast<float>(spl2[3])
					);
				}
				else
				{
					mat->uniformTextures.insert(std::pair<std::string, MaterialTexture>(spl[0], MaterialTexture(_resource->GetTexture(spl[1]))));
					lastTexture = spl[0];
				}
				break;
			}
		}
	}

	return mat;
}

template<>
std::unique_ptr<PostEffect> FilesystemManager::LoadFileResource(const FS::path& path)
{
	if(!FS::exists(path))
	{
		return std::unique_ptr<PostEffect>(nullptr);
	}

	std::vector<std::string> lines = ReturnFileLines(path, true);

	std::unique_ptr<PostEffect> postEffect = std::make_unique<PostEffect>();

	int readState = 0;//0 Properties, 1 Rendering Stages
	for(unsigned int i = 0; i < lines.size(); ++i)
	{
		if(lines[i] == "" || lines[i].substr(0, 2) == "//")
			continue;
		else if(lines[i][0] == '#')
		{
			if(lines[i] == "#PROPERTIES")
				readState = 0;
			if(lines[i] == "#STAGES")
				readState = 1;
		}
		else
		{
			std::vector<std::string> spl;
			switch(readState)
			{
			case 0:
				boost::split(spl, lines[i], boost::is_any_of("="), boost::token_compress_on);
				if(spl.size() < 2)
					continue;
				if(spl[0] == "cleanBufferBefore")
					postEffect->clearBuffersBefore = (spl[1] == "true");
				else if(spl[0] == "cleanBufferAfter")
					postEffect->clearBuffersAfter = (spl[1] == "true");
				else if(spl[0] == "order")
					postEffect->order = boost::lexical_cast<int>(spl[1]);
				break;
			case 1:
				boost::split(spl, lines[i], boost::is_any_of(":"), boost::token_compress_on);
				if(spl.size() < 2)
					continue;
				if(spl[1] == "null")
					postEffect->elementChain.push_back(std::pair<int, Material*>(boost::lexical_cast<int>(spl[0]), nullptr));
				else
					postEffect->elementChain.push_back(std::pair<int, Material*>(boost::lexical_cast<int>(spl[0]), _resource->GetMaterial(spl[1])));
				break;
			}
		}
	}

	return postEffect;
}

bool FilesystemManager::SaveFile(const FS::path& dir, std::string& content, int flags) const
{
	boost::algorithm::erase_all(content, "\r");

	std::ofstream ofs(dir.c_str(), flags);
	if(!ofs.is_open())
	{
		throw std::runtime_error("Unable to save resource at " + dir.string());
	}

	ofs << content;
	ofs.close();
	return true;
}

//Most of these parsers work in similar ways.
//Lines beginning with // are comments, #DIRECTIVES change where the data goes, the data itself is split on = and ,
void FilesystemManager::LoadControlSettings(const FS::path& path, std::unordered_map<InputDirection, InputEvent>& dir, std::unordered_map<InputButton, InputEvent>& bt) const
{
	std::vector<std::string> lines;

	if(!FS::exists(path))
	{
		lines = ReturnFileLines("Settings/Input/Default.vi");
		FS::copy_file("Settings/Input/Default.vi", path);
	}
	else
	{
		lines = ReturnFileLines(path);
	}

	dir.clear();
	bt.clear();

	int readState = 0;//0 Buttons, 1 Axes
	for(unsigned int i = 0; i < lines.size(); ++i)
	{
		if(lines[i] == "" || lines[i].substr(0, 2) == "//")
			continue;
		else if(lines[i][0] == '#')
		{
			if(lines[i] == "#BUTTONS")
				readState = 0;
			if(lines[i] == "#AXES")
				readState = 1;
		}
		else
		{
			std::vector<std::string> spl;
			std::vector<std::string> spl2;
			boost::split(spl, lines[i], boost::is_any_of(":"), boost::token_compress_on);
			boost::split(spl2, spl[1], boost::is_any_of(","), boost::token_compress_on);

			switch(readState)
			{
			case 0:
				bt.insert(std::pair<InputButton, InputEvent>((InputButton)boost::lexical_cast<int>(spl[0]), InputEvent(boost::lexical_cast<int>(spl2[1]), spl2[0] == "t", boost::lexical_cast<float>(spl2[2]), boost::lexical_cast<float>(spl2[3]))));
				break;
			case 1:
				dir.insert(std::pair<InputDirection, InputEvent>((InputDirection)boost::lexical_cast<int>(spl[0]), InputEvent(boost::lexical_cast<int>(spl2[1]), spl2[0] == "t", boost::lexical_cast<float>(spl2[2]), boost::lexical_cast<float>(spl2[3]))));
				break;
			}
		}
	}
}

void FilesystemManager::ApplyFunctionToFiles(const FS::path& dir, std::function<void(const FS::path&)> func) const
{
	if(!FS::is_directory(dir))
	{
		func(dir);
		return;
	}

	for(auto& iter : boost::make_iterator_range(FS::directory_iterator(dir)))
	{
		if(!FS::is_directory(iter.path()))
		{
			func(iter.path());
		}
	}
}

void FilesystemManager::Init()
{
	_debug = _serviceManager->Debug();
	_resource = _serviceManager->ResourceManager();
}

void FilesystemManager::Update()
{
}

void FilesystemManager::Cleanup()
{
}

void FilesystemManager::LoadTextureData(const FS::path& path, std::vector<unsigned char>& out_pixels, glm::ivec2& out_size) const
{
	out_size.x = out_size.y = 0;
	out_pixels.clear();

	unsigned char* pixels = SOIL_load_image(path.string().c_str(), &out_size.x, &out_size.y, nullptr, SOIL_LOAD_RGBA);
	if(pixels == nullptr)
	{
		_debug->VE_LOG("Unable to load texture at path " + path.string(), LogItem::Type::Warning);
		SOIL_free_image_data(pixels);
		return;
	}

	out_pixels.reserve(out_size.x * out_size.y * 4);
	for(int j = out_size.y - 1; j >= 0; --j)
	{
		for(int i = 0; i < out_size.x * 4; ++i)
		{
			out_pixels.push_back(pixels[j * out_size.x * 4 + i]);
		}
	}

	SOIL_free_image_data(pixels);
}


FilesystemManager::FilesystemManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{
}