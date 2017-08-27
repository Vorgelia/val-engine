#pragma once
#include "CommonUtilIncludes.hpp"

class Material;

//Buffer to write to , Material to render with
typedef std::pair<int, Material* > postEffectElement;

class PostEffect
{
public:
	std::vector<postEffectElement> elementChain;
	bool clearBuffersBefore;
	bool clearBuffersAfter;
	int order;
	PostEffect(std::vector<postEffectElement> elements, bool cbBefore = true, bool cbAfter = false, int order = 100);
	PostEffect(FS::path path);
};

