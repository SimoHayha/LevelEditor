#pragma once

#ifndef INC_FROM_MANAGED
#include <d3d11.h>
#include <DirectXTex.h>
#endif

class Texture
{
	enum E_FILETYPE
	{
		EDDS,
		ETGA,
		EWIC,
		ENONE
	};
	ID3D11ShaderResourceView* texture;
public:
	Texture(void);
	~Texture(void);

	bool initialize(ID3D11Device*, WCHAR*);
	void shutdown(void);

	ID3D11ShaderResourceView*	getTexture();
	E_FILETYPE					getTypeTexture(WCHAR* file);
	int							strcmpType(WCHAR* file, const char* type, int size);
	std::string					name;

};