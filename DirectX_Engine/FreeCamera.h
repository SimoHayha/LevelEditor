#pragma once

#include "MovementController.h"

class FreeCamera : public MovementController
{
public:
	FreeCamera();
	~FreeCamera();

	virtual bool	refresh(ID3D11DeviceContext* context, float dt);

	virtual	void	getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index);
	virtual int		getIndexCount(size_t);
	virtual size_t	getMeshCount();

	void	setRotation(float, float, float);
	void	addRotationSpeed(float, float, float);
	void	setRotationSpeed(float, float, float);
};