#pragma once
//Everything in here should be accessible virtually everywhere.
//System Utilities
#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include <vector>
#include <clocale>
#include <unordered_map>
#include <map>
#include <queue>
#include <typeinfo>
#include <thread>

//boost
#include <boost\filesystem.hpp>
#include <boost\lexical_cast.hpp>
#include <boost\algorithm\algorithm.hpp>
#include <boost\assign\list_of.hpp>
#include <boost\algorithm\string.hpp>
namespace FS = boost::filesystem;

//Math
#include "MathIncludes.hpp"