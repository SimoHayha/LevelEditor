#pragma once

#include "Entity.h"
#include "RayCaster.h"

class Arrow;

class Overlay : public Entity
{
public:
	enum class OverlayMode
	{
		E_BEGIN,
		E_TRANSFORM,
		E_ROTATE,
		E_SCALE,
		E_PLANE,
		E_LAST
	};

	enum class SelectedAxis
	{
		E_XAXIS,
		E_YAXIS,
		E_ZAXIS,
		E_NONE
	};

	Overlay();
	virtual ~Overlay();

	bool	initialize(ID3D11Device * device, ShaderManager * manager);
	bool	bind(Entity * father);
	void	onMouseMoved(Ray & ray);
	void	onMouseUp();
	bool	canRender() const;
	void	setMode(OverlayMode mode);
	void	nextMode();

	virtual int		getIndexCount(size_t);
	virtual size_t	getMeshCount();
	virtual void	getWorldMatrix(DirectX::XMMATRIX & newMatrix, float dt, size_t index);
	virtual void	childRender(ID3D11DeviceContext * context, float dt,
								DirectX::XMMATRIX& viewMatrix,
								DirectX::XMMATRIX& worldMatrix,
								DirectX::XMMATRIX& projectionMatrix,
								std::vector<DirectX::XMMATRIX*>& parentMatrices);

	Arrow &	getXAxis() const;
	Arrow &	getYAxis() const;
	Arrow &	getZAxis() const;

	SelectedAxis	getSelectedAxis(void);
	void	setSelectedAxis(SelectedAxis axis);
	bool	isAxisSelected() const;
	void	unselect();

	void	setSelectedObjectWorld(DirectX::XMFLOAT3X3 world);
	void	setActionMode(std::string & mode);

private:
	Arrow *			xAxis;
	Arrow *			yAxis;
	Arrow *			zAxis;

	bool			initialized;
	bool			binded;

	int				oldX;
	int				oldY;

	OverlayMode		mode;
	SelectedAxis	axis;

	bool	isPickedX(int sx, int sy);
	bool	isPickedY(int sx, int sy);
	bool	isPickedZ(int sx, int sy);

	DirectX::XMFLOAT3X3	selectedObjWorld;
};