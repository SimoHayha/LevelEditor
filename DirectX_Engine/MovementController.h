#pragma once

#include "Entity.h"

class MovementController : public Entity
{
public:
	enum class Type
	{
		E_FREECAM,
		E_2DCAM
	};
	
	MovementController();
	virtual ~MovementController();

	void	getPos(DirectX::XMFLOAT3 &);
	void	getUp(DirectX::XMFLOAT3 &);
	void	getRight(DirectX::XMFLOAT3 &);
	void	getDir(DirectX::XMFLOAT3 &);
	void	getV(DirectX::XMFLOAT3 &);
	void	getQuat(DirectX::XMFLOAT4 &);
	void	getViewMatrix(DirectX::XMMATRIX&);
	float	getThrust();
	void	setThrust(float);
	float	getRotX() const;
	float	getRotY() const;
	float	getRotZ() const;
	virtual bool	refresh(ID3D11DeviceContext* context, float dt);
	virtual bool	render(ID3D11DeviceContext *, DirectX::XMMATRIX &, DirectX::XMMATRIX &, DirectX::XMMATRIX &);
	void			reset();
	virtual void	onFeedback();
	int				viewport;

protected:
	Type				t;

	DirectX::XMMATRIX	viewMatrix;
	DirectX::XMFLOAT3	pos;
	DirectX::XMFLOAT3	up;
	DirectX::XMFLOAT3	right;
	DirectX::XMFLOAT3	dir;
	DirectX::XMFLOAT3	v;
	DirectX::XMFLOAT4	quat;

	float	pitchSpeed;
	float	rollSpeed;
	float	yawSpeed;

	float	pitchSpeedMax;
	float	rollSpeedMax;
	float	yawSpeedMax;

	float	rotX;
	float	rotY;
	float	rotZ;

	float	thrust;

	virtual void	recalcAxes();
	virtual void	init();

	Type	getType() const;
};