#pragma once
//Everything in here should be accessible virtually everywhere.
//System Utilities
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <locale.h>
#include <unordered_map>
#include <map>
#include <queue>
#include <typeinfo>
#include <thread>

//OpenGL
#include "GLIncludes.hpp"

//boost
#include <boost\filesystem.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\algorithm.hpp>
#include <boost\assign\list_of.hpp>
#include <boost\algorithm\string.hpp>
namespace FS = boost::filesystem;

//Files
#include <SOIL\src\SOIL.h>

//Math
#include<GLM\glm.hpp>
#include<GLM\gtc\matrix_transform.hpp>
#include<GLM\gtc\type_ptr.hpp>

//Profiler
#include "Profiler.h"