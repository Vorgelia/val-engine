#include "PostEffect.h"
#include "Material.h"
#include "ResourceLoader.h"
PostEffect::PostEffect(std::vector<postEffectElement> elements, bool cbBefore, bool cbAfter, int order)
{
	this->elementChain = elements;
	clearBuffersAfter = cbAfter;
	clearBuffersBefore = cbBefore;
	this->order = order;
}
PostEffect::PostEffect(FS::path path)
{
	ResourceLoader::LoadPostEffect(path, &elementChain, &clearBuffersBefore, &clearBuffersAfter, &order);
}