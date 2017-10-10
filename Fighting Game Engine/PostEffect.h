#pragma once
#include "CommonUtilIncludes.hpp"

class Material;

//Buffer to write to , Material to render with
typedef std::pair<int, Material* > PostEffectElement;

class PostEffect
{
public:
	std::vector<PostEffectElement> elementChain;
	bool clearBuffersBefore;
	bool clearBuffersAfter;
	int order;

	PostEffect(const std::vector<PostEffectElement>& elements, bool cbBefore = true, bool cbAfter = false, int order = 100);
	PostEffect(const FS::path& path);
};

