#include <DirectXMath.h>

#include "RayCaster.h"
#include "Arrow.h"
#include "Log.hpp"
#include "Overlay.h"

Overlay::Overlay() :
	xAxis(nullptr),
	yAxis(nullptr),
	zAxis(nullptr),
	initialized(false),
	binded(false),
	oldX(0),
	oldY(0),
	mode(OverlayMode::E_TRANSFORM),
	axis(SelectedAxis::E_NONE)
{
	_type = E_OVERLAY;
}

Overlay::~Overlay()
{
	delete this->xAxis;
	delete this->yAxis;
	delete this->zAxis;
}

bool	Overlay::initialize(ID3D11Device * device, ShaderManager * manager)
{
	try
	{
		this->xAxis = new Arrow();
		this->yAxis = new Arrow();
		this->zAxis = new Arrow();
	}
	catch (std::bad_alloc &)
	{
		delete this->xAxis;
		delete this->yAxis;
		delete this->zAxis;
	}

	if (false == this->xAxis->initializeBuffers(device) ||
		false == this->yAxis->initializeBuffers(device) ||
		false == this->zAxis->initializeBuffers(device))
		return false;

	if (false == this->xAxis->init(device) ||
		false == this->yAxis->init(device) ||
		false == this->zAxis->init(device))
		return false;

	this->xAxis->setShader(manager);
	this->yAxis->setShader(manager);
	this->zAxis->setShader(manager);

	if (false == this->appendChild(this->xAxis) ||
		false == this->appendChild(this->yAxis) ||
		false == this->appendChild(this->zAxis))
		return false;

	shaderMaterial*	mat = this->xAxis->getSubMeshMaterial(0);
	mat->ambient = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	mat->diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mat = this->yAxis->getSubMeshMaterial(0);
	mat->ambient = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	mat->diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	mat = this->zAxis->getSubMeshMaterial(0);
	mat->ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	mat->diffuse = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	this->initialized = true;
	return true;
}

void
Overlay::childRender(ID3D11DeviceContext * context, float dt,
					DirectX::XMMATRIX& viewMatrix,
					DirectX::XMMATRIX& worldMatrix,
					DirectX::XMMATRIX& projectionMatrix,
					std::vector<DirectX::XMMATRIX*>&)
{
	DirectX::XMMATRIX	world;
	DirectX::XMVECTOR	rot;
	DirectX::XMVECTOR	scale;
	DirectX::XMVECTOR	trans;

	world = worldMatrix;
	for (auto it : this->_childs)
	{
		this->yAxis->setRotateZ(DirectX::XM_PIDIV2);
		this->zAxis->setRotateY(-DirectX::XM_PIDIV2);
		it->getGlobalTransformMatrix(worldMatrix);
		DirectX::XMMatrixDecompose(&scale, &rot, &trans, worldMatrix);
		worldMatrix = DirectX::XMMatrixIdentity();
		if (it == this->yAxis)
			worldMatrix *= DirectX::XMMatrixRotationZ(DirectX::XM_PIDIV2);
		else if (it == this->zAxis)
			worldMatrix *= DirectX::XMMatrixRotationY(-DirectX::XM_PIDIV2);
		worldMatrix *= DirectX::XMMatrixTranslation(trans.m128_f32[0],
													trans.m128_f32[1],
													trans.m128_f32[2]);
		it->render(context, viewMatrix, worldMatrix, projectionMatrix);
		worldMatrix = world;
	}
}

bool	Overlay::bind(Entity * father)
{
	this->unselect();

	this->_father = father;

	if (nullptr == father)
	{
		this->binded = false;
		return true;
	}
	else
		this->binded = true;
	_father->setSelected(true);
	return true;
}

int	Overlay::getIndexCount(size_t)
{
	return 0;
}

size_t	Overlay::getMeshCount()
{
	return 1;
}

void	Overlay::getWorldMatrix(DirectX::XMMATRIX &, float, size_t)
{
}

bool	Overlay::canRender() const
{
	return (this->initialized && this->binded);
}

Arrow &	Overlay::getXAxis() const
{
	return *this->xAxis;
}

Arrow &	Overlay::getYAxis() const
{
	return *this->yAxis;
}

Arrow &	Overlay::getZAxis() const
{
	return *this->zAxis;
}

Overlay::SelectedAxis	Overlay::getSelectedAxis(void)
{
	return this->axis;
}

void	Overlay::setSelectedAxis(Overlay::SelectedAxis axis)
{
	this->xAxis->getSubMeshMaterial(0)->ambient = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	this->yAxis->getSubMeshMaterial(0)->ambient = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	this->zAxis->getSubMeshMaterial(0)->ambient = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	this->axis = axis;
	if (this->axis == SelectedAxis::E_XAXIS)
		this->xAxis->getSubMeshMaterial(0)->ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	else if (this->axis == SelectedAxis::E_YAXIS)
		this->yAxis->getSubMeshMaterial(0)->ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	else if (this->axis == SelectedAxis::E_ZAXIS)
		this->zAxis->getSubMeshMaterial(0)->ambient = DirectX::XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
}

void	Overlay::onMouseMoved(Ray & ray)
{
	using namespace DirectX;
	static float		oldValue = 0.0f;

	if (nullptr == this->_father)
		return;
	DirectX::XMMATRIX	worldPos = DirectX::XMMatrixIdentity();
	DirectX::XMVECTOR	axis;
	DirectX::XMVECTOR	scale;
	DirectX::XMVECTOR	rot;
	DirectX::XMVECTOR	pos;

	this->_father->getGlobalTransformMatrix(worldPos);
	DirectX::XMMatrixDecompose(&scale, &rot, &pos, worldPos);
	
	float				ori = ray.mOrigin.m128_f32[static_cast<int>(this->axis)];
	float				dir = ray.mDest.m128_f32[static_cast<int>(this->axis)];
	float				d;
	float				t;
	DirectX::XMVECTOR	normal;
	if (this->axis == Overlay::SelectedAxis::E_XAXIS)
	{
		normal = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f));
		axis = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f));
		d = DirectX::XMVector3Dot(normal, pos).m128_f32[0];
		t = ((-d - ray.mOrigin.m128_f32[2]) / ray.mDest.m128_f32[2]);
	}
	else if (this->axis == Overlay::SelectedAxis::E_YAXIS)
	{
		normal = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f));
		axis = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f));
		d = DirectX::XMVector3Dot(normal, pos).m128_f32[0];
		t = ((-d - ray.mOrigin.m128_f32[2]) / ray.mDest.m128_f32[2]);
	}
	else if (this->axis == Overlay::SelectedAxis::E_ZAXIS)
	{
		normal = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f));
		axis = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
		d = DirectX::XMVector3Dot(normal, pos).m128_f32[0];
		t = ((-d - ray.mOrigin.m128_f32[0]) / ray.mDest.m128_f32[0]);
	}
	float value = ori + dir * t;

	if (OverlayMode::E_TRANSFORM == this->mode)
	{
		if (this->axis == Overlay::SelectedAxis::E_XAXIS)
			this->_father->setTransX(value - 0.5f);
		else if (this->axis == Overlay::SelectedAxis::E_YAXIS)
			this->_father->setTransY(value - 0.5f);
		else if (this->axis == Overlay::SelectedAxis::E_ZAXIS)
			this->_father->setTransZ(value - 0.5f);
	}

	if (OverlayMode::E_SCALE == this->mode)
	{
		DirectX::XMVECTOR	forward;
		DirectX::XMVECTOR	up;
		DirectX::XMVECTOR	right;

		this->_father->getForward(forward);
		this->_father->getUp(up);
		this->_father->getRight(right);
		float coeffX = DirectX::XMVector3Dot(forward, axis).m128_f32[0];
		float coeffY = DirectX::XMVector3Dot(right, axis).m128_f32[0];
		float coeffZ = DirectX::XMVector3Dot(up, axis).m128_f32[0];
		this->_father->setScaleX(this->_father->getScaleX() + (value - oldValue) * coeffX);
		this->_father->setScaleY(this->_father->getScaleY() + (value - oldValue) * coeffY);
		this->_father->setScaleZ(this->_father->getScaleZ() + (value - oldValue) * coeffZ);
	}
	if (OverlayMode::E_ROTATE == this->mode)
	{
		if (this->axis == Overlay::SelectedAxis::E_XAXIS)
			this->_father->setRotateX(this->_father->getRotX() + value - oldValue);
		else if (this->axis == Overlay::SelectedAxis::E_YAXIS)
			this->_father->setRotateY(this->_father->getRotY() + value - oldValue);
		else if (this->axis == Overlay::SelectedAxis::E_ZAXIS)
			this->_father->setRotateZ(this->_father->getRotZ() + value - oldValue);
	}
	oldValue = value;
 }

void	Overlay::onMouseUp()
{
	this->axis = Overlay::SelectedAxis::E_NONE;
}

bool	Overlay::isAxisSelected() const
{
	return this->axis != Overlay::SelectedAxis::E_NONE;
}

void	Overlay::nextMode()
{
	int	e = static_cast<int>(this->mode);

	e++;

	this->mode = static_cast<Overlay::OverlayMode>(e);

	if (Overlay::OverlayMode::E_LAST == this->mode)
		this->mode = static_cast<Overlay::OverlayMode>(static_cast<int>(Overlay::OverlayMode::E_BEGIN) + 1);
}

void	Overlay::setSelectedObjectWorld(DirectX::XMFLOAT3X3 world)
{
	logCall(log_level_t::LOG_INFO) << "Setting object world matrix" << Elog::flush;
	this->selectedObjWorld = world;
}

void	Overlay::unselect()
{
	if (nullptr != this->_father)
		this->_father->setSelected(false);
	this->_father = nullptr;
	this->binded = false;
}

void	Overlay::setActionMode(std::string & mode)
{
	if ("Move" == mode)
		this->mode = Overlay::OverlayMode::E_TRANSFORM;
	else if ("Rotate" == mode)
		this->mode = Overlay::OverlayMode::E_ROTATE;
	else if ("Scale" == mode)
		this->mode = Overlay::OverlayMode::E_SCALE;
	else if ("Plane" == mode)
		this->mode = Overlay::OverlayMode::E_PLANE;
}