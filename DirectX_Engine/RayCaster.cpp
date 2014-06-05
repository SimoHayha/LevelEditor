#include "RayCaster.h"

Ray::Ray()
{
}

Ray::Ray(DirectX::XMVECTOR origin, DirectX::XMVECTOR dest) :
	mOrigin(origin),
	mDest(dest)
{
}

Ray::~Ray()
{
}

Ray	RayCaster::getRay(float vx,
					  float vy,
					  DirectX::XMMATRIX & invView,
					  DirectX::XMMATRIX & invWorld,
					  Entity & entity)
{
	using namespace DirectX;

	XMMATRIX				W;
	XMMATRIX				toLocal;
	XMVECTOR				rayOrigin = XMLoadFloat3(&XMFLOAT3(0.0f, 0.0f, 0.0f));
	XMVECTOR				rayDir    = XMLoadFloat3(&XMFLOAT3(vx, vy, 1.0f));
	
	W = DirectX::XMMatrixIdentity();

	entity.getGlobalTransformMatrix(W);

	invWorld = XMMatrixInverse(&XMMatrixDeterminant(W), W);

	toLocal = XMMatrixMultiply(invView, invWorld);

	rayOrigin = XMVector3TransformCoord(rayOrigin, toLocal);
	rayDir = XMVector3TransformNormal(rayDir, toLocal);

	rayDir = XMVector3Normalize(rayDir);

	return Ray(rayOrigin, rayDir);
}

void
RayCaster::getRay(int sx, int sy, DirectX::XMMATRIX& proj, DirectX::XMMATRIX& view, DirectX::XMMATRIX& world,
				  DirectX::XMVECTOR& camPos, D3D11_VIEWPORT * viewport, Ray& ray)
{
	DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(static_cast<float>(sx), static_cast<float>(sy), 1.0f));
	DirectX::XMVECTOR unprojected = DirectX::XMVector3Unproject(pos, viewport->TopLeftX, viewport->TopLeftY,
																viewport->Width, viewport->Height, viewport->MinDepth,
 																viewport->MaxDepth, proj, view, world);
	unprojected = DirectX::XMVector3Normalize(unprojected);
	ray = Ray(camPos, unprojected);
}

bool	RayCaster::intersect(Ray & ray, Entity & entity, float & distance)
{
	DirectX::BoundingBox	aabb;
	float					tmin;

	if (entity.hasAabb())
		aabb = entity.getAabb();
	else
		return false;

	if (aabb.Intersects(ray.mOrigin, ray.mDest, tmin))
	{
		if (tmin < distance)
		{
			distance = tmin;
			return true;
		}
	}

	return false;
}