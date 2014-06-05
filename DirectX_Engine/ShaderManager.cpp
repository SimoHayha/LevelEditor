#include "ShaderManager.h"

ShaderManager::ShaderManager() :
	textureShader(nullptr)
{
}

ShaderManager::~ShaderManager()
{
}

bool
ShaderManager::init(ID3D11Device* device, HWND hwnd)
{
	textureShader = new (std::nothrow) TextureShader();
	if (!textureShader || textureShader->initialize(device, hwnd) == false)
		return false;
	return true;
}

void
ShaderManager::shutdown(void)
{
	if (textureShader)
	{
		textureShader->shutdown();
		delete textureShader;
	}
}

TextureShader*
ShaderManager::getTextureShader(void)
{
	return textureShader;
}