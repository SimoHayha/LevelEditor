#pragma once
#include "Entity.h"
#include <vector>

class Light : public Entity
{
public:
	struct shaderLight
	{
		DirectX::XMFLOAT4	position;
		DirectX::XMFLOAT4	color;
	};
private:
	shaderLight*	light;
public:
	Light(void);
	~Light(void);
	bool						initialize(void);
	shaderLight*				getLight(void);	
	virtual int					getIndexCount(size_t);
	virtual size_t				getMeshCount(void);
	virtual	void				getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index);
	virtual bool				render(ID3D11DeviceContext* context,
									   DirectX::XMMATRIX& viewMatrix,
									   DirectX::XMMATRIX& worldMatrix,
					   				   DirectX::XMMATRIX& projectionMatrix);
	

};

