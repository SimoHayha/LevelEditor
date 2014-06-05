#pragma once

#include "Entity.h"

class Container : public Entity
{
public:
	Container();
	virtual ~Container();

	bool			initialize(ID3D11Device * device);
	int				getIndexCount(size_t index);
	size_t			getMeshCount();
	void			getWorldMatrix(DirectX::XMMATRIX & newMatrix, float dt, size_t index);
	virtual bool	refresh(ID3D11DeviceContext* context, float dt);
	bool			render(ID3D11DeviceContext *, DirectX::XMMATRIX &, DirectX::XMMATRIX &, DirectX::XMMATRIX &);
	virtual bool	appendChild(Entity * child);

private:
	ID3D11Device *	_device;
};