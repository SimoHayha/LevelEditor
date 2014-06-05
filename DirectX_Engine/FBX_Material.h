#pragma once

#include <fbxsdk.h>
#include <DirectXMath.h>

#include "Texture.h"
#include "Entity.h"

namespace id
{
	namespace FBX
	{
		enum EMat
		{
			EEMMISIVE = 0,
			EAMBIENT,
			EDIFFUSE,
			ESPECULAR
		};
		
		class Material
		{
		public:
			Material(void);
			~Material(void);
			bool	init(FbxSurfaceMaterial* inMat);
			bool	getMaterialInfo(FbxSurfaceMaterial* inMat, DirectX::XMFLOAT4& value,
									const char* propName, const char* propFactorName, int index);
			bool	shutdown(void);
			ID3D11ShaderResourceView*	getTexture(size_t index);
			Texture*					getTextureContainer(size_t index);
			void						setTextureContainer(size_t index, Texture* texture);
			shaderMaterial*		getShaderMaterial(void);
			FbxSurfaceMaterial*	getFbxMaterial(void);

		private:
			shaderMaterial		_mat;
			Texture*			_texture[4];
			FbxSurfaceMaterial*	_fbxMat;

		};
	}
}