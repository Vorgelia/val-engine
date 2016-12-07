#pragma once
//Everything in here should be accessible virtually everywhere.
//System Utilities
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale.h>
#include <unordered_map>
#include <map>
#include <typeinfo>
#include <thread>
//OpenGL
#define GLEW_STATIC
#define GLFW_INCLUDE_GLU
#include <GLEW\GL\glew.h>
#include <GLFW\include\GLFW\glfw3.h>
//boost
#include <boost\filesystem.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\algorithm.hpp>
#include <boost\assign\list_of.hpp>
#include <boost\algorithm\string\classification.hpp> 
#include <boost\algorithm\string\split.hpp>
#include <boost\algorithm\string\erase.hpp>
namespace FS = boost::filesystem;
//Files
#include <SOIL\src\SOIL.h>
//Math
#include<GLM\glm.hpp>
#include<GLM\gtc\matrix_transform.hpp>
#include<GLM\gtc\type_ptr.hpp>
//Text
#include<ft2build.h>
#include FT_FREETYPE_H
//Profiler
#include "Profiler.h"