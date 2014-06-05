#include "Light.h"

Light::Light(void) : light(nullptr)
{
	_type = Entity::E_LIGHT;
}


Light::~Light(void)
{
	delete	light;
}

bool
Light::initialize(void)
{
	light = new (std::nothrow) shaderLight();
	if (light == nullptr)
		return false;
	try
	{
		_name = "Light";
		_defaultMatName = "defaultMatLight";
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	getLight();
	return true;
}


int
Light::getIndexCount(size_t index)
{
	return 0;
}

size_t
Light::getMeshCount(void)
{
	return 0;
}

void
Light::getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index)
{
}

bool
Light::render(ID3D11DeviceContext* context,
		      DirectX::XMMATRIX& viewMatrix,
			  DirectX::XMMATRIX& worldMatrix,
			  DirectX::XMMATRIX& projectionMatrix)
{
	return true;
}

Light::shaderLight*
Light::getLight(void)
{
	DirectX::XMMATRIX transform = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR rot;
	DirectX::XMVECTOR scale;
	DirectX::XMVECTOR trans;

	getGlobalTransformMatrix(transform);
	DirectX::XMMatrixDecompose(&scale, &rot, &trans, transform);
	DirectX::XMStoreFloat4(&light->position, trans);
	light->color = DirectX::XMFLOAT4(_defaultMat.color.x, _defaultMat.color.y, _defaultMat.color.z, 0.0f);
	return light;
}