#include "FreeCamera.h"

FreeCamera::FreeCamera()
{
}

FreeCamera::~FreeCamera()
{
}

void	FreeCamera::setRotation(float x, float y, float z)
{
	this->_transform.rotX = x;
	this->_transform.rotY = y;
	this->_transform.rotZ = z;
	this->recalcAxes();
}

void	FreeCamera::addRotationSpeed(float x, float y, float z)
{
	this->pitchSpeed += x;
	this->yawSpeed   += y;
	this->rollSpeed  += z;
}

void	FreeCamera::setRotationSpeed(float x, float y, float z)
{
	this->pitchSpeed = x;
	this->yawSpeed   = y;
	this->rollSpeed  = z;
}

bool	FreeCamera::refresh(ID3D11DeviceContext* context, float dt)
{
	this->rotX = (this->pitchSpeed * dt) / 4.0f;
	this->rotY = (this->yawSpeed * dt) / 4.0f;
	this->rotZ = (this->rollSpeed * dt) / 4.0f;

	this->v.x = this->dir.x * dt;
	this->v.y = this->dir.y * dt;
	this->v.z = this->dir.z * dt;

	this->pos.x += this->v.x * this->thrust;
	this->pos.y += this->v.y * this->thrust;
	this->pos.z += this->v.z * this->thrust;

	this->recalcAxes();

	using namespace DirectX;

	XMVECTOR	lookAt;
	XMVECTOR	pos  = XMLoadFloat3(&this->pos);
	XMVECTOR	forw = XMLoadFloat3(&this->dir);
	XMVECTOR	up   = XMLoadFloat3(&this->up);

	lookAt = XMVectorAdd(pos, forw);

	this->viewMatrix = XMMatrixLookAtLH(pos, lookAt, up);

	this->_transform.transX = this->pos.x;
	this->_transform.transY = this->pos.y;
	this->_transform.transZ = this->pos.z;
	this->_transform.rotX = this->rotX;
	this->_transform.rotY = this->rotY;
	this->_transform.rotZ = this->rotZ;

	//this->_unmanaged->_feedback->OnCameraMoved();
	return true;
}

int		FreeCamera::getIndexCount(size_t)
{
	return 0;
}

size_t	FreeCamera::getMeshCount()
{
	return 0;
}

void	FreeCamera::getWorldMatrix(DirectX::XMMATRIX&, float, size_t)
{
}