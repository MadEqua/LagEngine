#pragma once
class Texture
{
public:
	Texture();
	~Texture();

	virtual void setTextureWrap() = 0; //TODO: all the texture stuff
};

