#include "PostEffect.h"
#include "Material.h"
#include "ResourceLoader.h"

PostEffect::PostEffect(const std::vector<PostEffectElement>& elements, bool cbBefore, bool cbAfter, int order)
{
	elementChain = elements;
	clearBuffersAfter = cbAfter;
	clearBuffersBefore = cbBefore;
	this->order = order;
}

PostEffect::PostEffect(const FS::path& path)
{
	ResourceLoader::LoadPostEffect(path, elementChain, clearBuffersBefore, clearBuffersAfter, order);
}