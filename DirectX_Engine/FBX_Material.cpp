#include "FBX_Material.h"
#include "Dictionnary.h"

id::FBX::Material::Material()
{
	ZeroMemory(&_mat, sizeof(_mat));
	ZeroMemory(_texture, sizeof(*_texture)* 4);
}

id::FBX::Material::~Material()
{
}

bool
id::FBX::Material::init(FbxSurfaceMaterial* inMat)
{
	FbxProperty	shiny = inMat->FindProperty(FbxSurfaceMaterial::sShininess);

	_fbxMat = inMat;
	_mat.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	getMaterialInfo(inMat, _mat.emmisive, FbxSurfaceMaterial::sEmissive, FbxSurfaceMaterial::sEmissiveFactor, EEMMISIVE);
	getMaterialInfo(inMat, _mat.ambient, FbxSurfaceMaterial::sAmbient, FbxSurfaceMaterial::sAmbientFactor, EAMBIENT);
	getMaterialInfo(inMat, _mat.diffuse, FbxSurfaceMaterial::sDiffuse, FbxSurfaceMaterial::sDiffuseFactor, EDIFFUSE);
	getMaterialInfo(inMat, _mat.specular, FbxSurfaceMaterial::sSpecular, FbxSurfaceMaterial::sSpecularFactor, ESPECULAR);
	if (shiny.IsValid())
		_mat.shinness = shiny.Get<float>();
	return true;
}

bool
id::FBX::Material::getMaterialInfo(FbxSurfaceMaterial* inMat, DirectX::XMFLOAT4& value,
									const char* propName, const char* propFactorName, int index)
{
	const FbxProperty	prop = inMat->FindProperty(propName);
	const FbxProperty	propFactor = inMat->FindProperty(propFactorName);
	FbxFileTexture*		text;
	FbxDouble3			tmp;
	double				factor;

	if (prop.IsValid() && propFactor.IsValid())
	{
		tmp = prop.Get<FbxDouble3>();
		factor = propFactor.Get<double>();
		value.x = static_cast<float>(tmp[0] * factor);
		value.y = static_cast<float>(tmp[0] * factor);
		value.z = static_cast<float>(tmp[0] * factor);
	}
	if (prop.IsValid() && prop.GetSrcObjectCount<FbxFileTexture>())
	{
		text = prop.GetSrcObject<FbxFileTexture>();
		_texture[index] = Dictionnary::getInstance()->getTexture(text);
	}
	return true;
}

ID3D11ShaderResourceView*
id::FBX::Material::getTexture(size_t index)
{
	if (_texture[index] == nullptr)
		return nullptr;
	return _texture[index]->getTexture();
}

Texture*
id::FBX::Material::getTextureContainer(size_t index)
{
	return _texture[index];
}

void
id::FBX::Material::setTextureContainer(size_t index, Texture* texture)
{
	_texture[index] = texture;
}

shaderMaterial*
id::FBX::Material::getShaderMaterial(void)
{
	return &_mat;
}

FbxSurfaceMaterial*
id::FBX::Material::getFbxMaterial(void)
{
	return _fbxMat;
}