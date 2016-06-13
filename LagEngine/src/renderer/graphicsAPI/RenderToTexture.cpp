#include "RenderToTexture.h"
#include "../../resources/Image.h"

using namespace Lag;

RenderToTexture::RenderToTexture(uint32 width, uint32 height) :
	RenderTarget(width, height)
{
}

RenderToTexture::~RenderToTexture()
{
}