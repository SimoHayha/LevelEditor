#pragma once

#include "MovementController.h"

class FpsCam : public MovementController
{
public:
	FpsCam();
	virtual ~FpsCam();

	virtual bool		refresh(ID3D11DeviceContext* context, float dt);

	void				getRotation(float *, float *, float *) const;
	DirectX::XMFLOAT3	getRotation() const;

	virtual	void		getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index);
	virtual int			getIndexCount(size_t);
	virtual size_t		getMeshCount();

	void				setRotationSpeedX(float f);
	void				setRotationSpeedY(float f);
	void				setSpeed(float f);
	void				setSlideSpeed(float f);
	void				setCrouchSpeed(float f);

	void				setRotation(float, float, float);


private:
	float				speed;
	float				slide;
	float				crouch;
	float				coef;

	virtual void		recalcAxes();
};