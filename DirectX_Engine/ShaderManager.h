#pragma once

#include <d3d11.h>
#include "TextureShader.h"
#include "Entity.h"
#include "Model.h"

class ShaderManager
{
public:
	ShaderManager(void);
	~ShaderManager(void);

	bool			init(ID3D11Device* device, HWND hwnd);
	void			shutdown(void);
	TextureShader*	getTextureShader(void);

private:
	TextureShader*	textureShader;

};