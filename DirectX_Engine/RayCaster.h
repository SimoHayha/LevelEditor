#pragma once

#include <DirectXMath.h>
#include <DirectXCollision.h>

#include "Entity.h"

class Ray
{
public:
	Ray();
	Ray(DirectX::XMVECTOR origin, DirectX::XMVECTOR dest);
	~Ray();

	DirectX::XMVECTOR	mOrigin;
	DirectX::XMVECTOR	mDest;
};

class RayCaster
{
public:
	static Ray	getRay(float vx, float vy, DirectX::XMMATRIX & invView, DirectX::XMMATRIX & invWorld, Entity & entity);
	static void	getRay(int sx, int sy, DirectX::XMMATRIX& proj, DirectX::XMMATRIX& view, DirectX::XMMATRIX& world,
					   DirectX::XMVECTOR& origin, D3D11_VIEWPORT * viewport, Ray& ray);
	static bool	intersect(Ray & ray, Entity & entity, float & distance);

private:
	RayCaster();
	~RayCaster();
};