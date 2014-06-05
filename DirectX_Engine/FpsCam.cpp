#include "FpsCam.h"

FpsCam::FpsCam()
{
	this->speed	 = 0.0f;
	this->slide	 = 0.0f;
	this->crouch = 0.0f;
	this->coef   = 1.0f;
}

FpsCam::~FpsCam()
{
}

void	FpsCam::setRotation(float rx, float ry, float rz)
{
	this->rotX = rx;
	this->rotY = ry;
	this->rotZ = rz;
	this->recalcAxes();
}

void	FpsCam::getRotation(float * fx, float * fy, float * fz) const
{
	if (fx)
		*fx = this->rotX;
	if (fy)
		*fy = this->rotY;
	if (fz)
		*fz = this->rotZ;
}

DirectX::XMFLOAT3	FpsCam::getRotation() const
{
	return DirectX::XMFLOAT3(this->rotX, this->rotY, this->rotZ);
}

void	FpsCam::recalcAxes()
{
	using namespace DirectX;

	XMMATRIX	mat;

	if (this->rotY > XM_2PI)
		this->rotY -= XM_2PI;
	else if (this->rotY < -XM_2PI)
		this->rotY += XM_2PI;

	if (this->rotX > 1.4f)
		this->rotX = 1.4f;
	else if (this->rotX < -1.4f)
		this->rotX = -1.4f;

	this->right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	this->up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	this->dir = XMFLOAT3(0.0f, 0.0f, 1.0f);
	XMVECTOR	vright(XMLoadFloat3(&this->right));
	XMVECTOR	vup(XMLoadFloat3(&this->up));
	XMVECTOR	vdir(XMLoadFloat3(&this->dir));

	/* Rotate around y-axis */
	mat = XMMatrixRotationAxis(vup, this->rotY);
	vright = XMVector3Transform(vright, mat);
	vdir = XMVector3Transform(vdir, mat);

	/* Rotate around x-axis */
	mat = XMMatrixRotationAxis(vright, this->rotX);
	vup = XMVector3Transform(vup, mat);
	vdir = XMVector3Transform(vdir, mat);

	/* Correct rounding error */
	vdir = XMVector3Normalize(vdir);
	vright = XMVector3Cross(vup, vdir);
	vright = XMVector3Normalize(vright);
	vup = XMVector3Cross(vdir, vright);
	vup = XMVector3Normalize(vup);

	XMStoreFloat3(&this->up, vup);
	XMStoreFloat3(&this->dir, vdir);
	XMStoreFloat3(&this->right, vright);
}

bool	FpsCam::refresh(ID3D11DeviceContext*, float ft)
{
	using namespace DirectX;

	XMFLOAT3	s;
	XMFLOAT3	u;

	this->rotX += (this->pitchSpeed * ft) / 4.0f;
	this->rotY += (this->yawSpeed * ft) / 4.0f;
	this->rotZ += (this->rollSpeed * ft) / 4.0f;

	this->recalcAxes();

	this->v.x = this->dir.x * this->speed * ft * this->coef;
	this->v.y = this->dir.y * this->speed * ft * this->coef;
	this->v.z = this->dir.z * this->speed * ft * this->coef;

	s.x = this->right.x * this->slide * ft * this->coef;
	s.y = this->right.y * this->slide * ft * this->coef;
	s.z = this->right.z * this->slide * ft * this->coef;

	u.x = this->up.x * this->crouch * ft * this->coef;
	u.y = this->up.y * this->crouch * ft * this->coef;
	u.z = this->up.z * this->crouch * ft * this->coef;

	this->pos.x += this->v.x + s.x + u.x;
	this->pos.y += this->v.y + s.y + u.y;
	this->pos.z += this->v.z + s.z + u.z;

	this->_transform.transX = this->pos.x;
	this->_transform.transY = this->pos.y;
	this->_transform.transZ = this->pos.z;
	this->_transform.rotX = this->rotX;
	this->_transform.rotY = this->rotY;
	this->_transform.rotZ = this->rotZ;

	using namespace DirectX;

	XMVECTOR	lookAt;
	XMVECTOR	pos  = XMLoadFloat3(&this->pos);
	XMVECTOR	forw = XMLoadFloat3(&this->dir);
	XMVECTOR	up   = XMLoadFloat3(&this->up);

	lookAt = XMVectorAdd(pos, forw);

	this->viewMatrix = XMMatrixLookAtLH(pos, lookAt, up);

	//this->_unmanaged->_feedback->OnCameraMoved();
	return true;
}

int	FpsCam::getIndexCount(size_t)
{
	return 0;
}

void	FpsCam::getWorldMatrix(DirectX::XMMATRIX &, float dt, size_t)
{
}

size_t	FpsCam::getMeshCount()
{
	return 0;
}

void	FpsCam::setRotationSpeedX(float f)
{
	this->pitchSpeed = f;
}

void	FpsCam::setRotationSpeedY(float f)
{
	this->yawSpeed = f;
}

void	FpsCam::setSpeed(float f)
{
	this->speed = f;
}

void	FpsCam::setSlideSpeed(float f)
{
	this->slide = f;
}

void	FpsCam::setCrouchSpeed(float f)
{
	this->crouch = f;
}