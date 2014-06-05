#include "MovementController.h"

MovementController::MovementController() : t(MovementController::Type::E_FREECAM)
{
	_type = Entity::E_CAMERA;
	viewport = -1;
	this->init();
}

MovementController::~MovementController()
{
}

void	MovementController::init()
{
	using namespace DirectX;

	this->_type = Entity::E_CAMERA;
	this->pos = XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->up = XMFLOAT3(0.0f, 1.0f, 0.0f);
	this->right = XMFLOAT3(1.0f, 0.0f, 0.0f);
	this->quat = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->pitchSpeed = this->rollSpeed = this->yawSpeed = 0.0f;
	this->rotX = this->rotY = this->rotZ = 0.0f;
	this->thrust = 0.0f;
	_name = "camera";
}

void	MovementController::recalcAxes()
{
	using namespace DirectX;

	XMMATRIX	mat;
	XMVECTOR	frame;
	XMVECTOR	lquat;

	if (this->rotX > XM_2PI)
		this->rotX -= XM_2PI;
	else if (this->rotX < -XM_2PI)
		this->rotX += XM_2PI;

	if (this->rotY > XM_2PI)
		this->rotY -= XM_2PI;
	else if (this->rotY < -XM_2PI)
		this->rotY += XM_2PI;

	if (this->rotZ > XM_2PI)
		this->rotZ -= XM_2PI;
	else if (this->rotZ < -XM_2PI)
		this->rotZ += XM_2PI;

	frame = XMQuaternionRotationRollPitchYaw(this->rotX, this->rotY, this->rotZ);
	lquat = XMLoadFloat4(&this->quat);

	lquat = XMQuaternionMultiply(lquat, frame);

	XMStoreFloat4(&this->quat, lquat);
	mat = XMMatrixRotationQuaternion(lquat);

	this->right.x = mat.r[0].m128_f32[0];
	this->right.y = mat.r[1].m128_f32[0];
	this->right.z = mat.r[2].m128_f32[0];

	this->up.x = mat.r[0].m128_f32[1];
	this->up.y = mat.r[1].m128_f32[1];
	this->up.z = mat.r[2].m128_f32[1];

	this->dir.x = mat.r[0].m128_f32[2];
	this->dir.y = mat.r[1].m128_f32[2];
	this->dir.z = mat.r[2].m128_f32[2];
}

void	MovementController::getPos(DirectX::XMFLOAT3 & f)
{
	f = this->pos;
}

void	MovementController::getUp(DirectX::XMFLOAT3 & f)
{
	f = this->up;
}

void	MovementController::getRight(DirectX::XMFLOAT3 & f)
{
	f = this->right;
}

void	MovementController::getDir(DirectX::XMFLOAT3 & f)
{
	f = this->dir;
}

void	MovementController::getV(DirectX::XMFLOAT3 & f)
{
	f = this->v;
}

void	MovementController::getQuat(DirectX::XMFLOAT4 & f)
{
	f = this->quat;
}

void	MovementController::getViewMatrix(DirectX::XMMATRIX & view)
{
	view = this->viewMatrix;
}

MovementController::Type	MovementController::getType() const
{
	return this->t;
}

float	MovementController::getThrust()
{
	return this->thrust;
}

void	MovementController::setThrust(float f)
{
	this->thrust = f;
}

float	MovementController::getRotX() const
{
	return this->rotX;
}

float	MovementController::getRotY() const
{
	return this->rotY;
}

float	MovementController::getRotZ() const
{
	return this->rotZ;
}

void	MovementController::reset()
{
	this->yawSpeed = 0.0f;
	this->pitchSpeed = 0.0f;
	this->rollSpeed = 0.0f;
	this->thrust = 0.0f;
}

void	MovementController::onFeedback()
{
	Entity::onFeedback();

	this->_unmanaged->_feedback->OnCameraMoved();
}

bool	MovementController::refresh(ID3D11DeviceContext* context, float dt)
{
	return true;
}

bool	MovementController::render(ID3D11DeviceContext *, DirectX::XMMATRIX &, DirectX::XMMATRIX &, DirectX::XMMATRIX &)
{
	return true;
}