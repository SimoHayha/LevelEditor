#include "Texture.h"

Texture::Texture(void) :
	texture(nullptr)
{

}

Texture::~Texture(void)
{

}

#include <stdio.h>
#include <windows.h>
#include <tchar.h>

void ErrorDescription(HRESULT hr) 
{ 
     if(FACILITY_WINDOWS == HRESULT_FACILITY(hr)) 
         hr = HRESULT_CODE(hr); 
     TCHAR* szErrMsg; 

     if(FormatMessage( 
       FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, 
       NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
       (LPTSTR)&szErrMsg, 0, NULL) != 0) 
     { 
		 OutputDebugStringW(szErrMsg);
         _tprintf(TEXT("%s"), szErrMsg); 
         LocalFree(szErrMsg); 
     } else 
         _tprintf( TEXT("[Could not find a description for error # %#x.]\n"), hr); 
}

bool
Texture::initialize(ID3D11Device* device, WCHAR* file)
{
	DirectX::ScratchImage	img;
	DirectX::TexMetadata	info;
	E_FILETYPE				type;

	type = getTypeTexture(file);
	switch (type)
	{
		case EDDS:
			if (FAILED(DirectX::LoadFromDDSFile(file, DirectX::DDS_FLAGS_NONE, &info, img)))
				return false;
			break;
		case ETGA:
			if (FAILED(DirectX::LoadFromTGAFile(file, &info, img)))
				return (false);
			break;
		case EWIC:
			if (FAILED(DirectX::LoadFromWICFile(file, DirectX::WIC_FLAGS_NONE, &info, img)))
				return (false);
			break;
		default:
			return (false);
			break;
	}
	if (FAILED(DirectX::CreateShaderResourceView(device, img.GetImages(), img.GetImageCount(), info, &texture)))
		return false;
	return true;
}

int	Texture::strcmpType(WCHAR* file, const char* type, int size)
{
	int	j = 0;

	for (int i = size - 4; type[j] != '\0' && file[i] != '\0'; ++i)
	{
		if (type[j] != file[i])
			return (-1);
		++j;
	}
	return (0);
}

Texture::E_FILETYPE	Texture::getTypeTexture(WCHAR* file)
{
	int	size;

	for (size = 0; file[size] != '\0'; ++size)
		;
	if (size < 4)
		return (ENONE);
	if (!strcmpType(file, ".tga", size))
		return (ETGA);
	if (!strcmpType(file, ".dds", size))
		return (EDDS);
	return (EWIC);
}

void
Texture::shutdown(void)
{
	if (texture)
	{
		texture->Release();
		texture = nullptr;
	}
}

ID3D11ShaderResourceView*
Texture::getTexture(void)
{
	return texture;
}