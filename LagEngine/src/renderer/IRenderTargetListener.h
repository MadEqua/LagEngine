#pragma once

class IRenderTargetListener
{
public:	
	virtual ~IRenderTargetListener() {}

	virtual void onPreRender() = 0;
	virtual void onPostRender() = 0;
};