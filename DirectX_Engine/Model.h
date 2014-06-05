#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>
#include <fbxsdk.h>
#include <vector>

#include "Entity.h"
#include "FBX_Model.h"
#include "FBX_Animator.h"
#include "Texture.h"
#include "Buffers.h"

class Model : public Entity
{
private:
	bool						buildBoundingBox();

	id::FBX::Model*				model;
	id::FBX::Animator			animator;
	DirectX::XMMATRIX			worldMatrix;
	ShaderManager*				_shaders;
	bool						_initialized;
	size_t						_meshIndex;
	bool						animate;
	bool						animateOneTime;
	int							currentAnimation;
	float						animTimeCallback;
	ID3D11Device *				device;
	bool						animAutoNext;

public:
			Model(void);
	bool	initialize(ID3D11Device*, id::FBX::Model* fbxModel, Buffers* buffer, size_t meshIndex);
	void	setShader(ShaderManager* shader);
	int		getIndexCount(size_t index);
	void	getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index);
	size_t	getMeshCount(void);
	bool	refresh(ID3D11DeviceContext* context, float dt);
	bool	render(ID3D11DeviceContext* context,
				   DirectX::XMMATRIX& viewMatrix,
				   DirectX::XMMATRIX& worldMatrix,
				   DirectX::XMMATRIX& projectionMatrix);

	id::FBX::Animator const *	getAnimator() const;

	/* Com with c++/cli */
	void					setAnimate(bool b);
	bool					isAnimate() const;
	int						getCurrentAnimation() const;
	void					setCurrentAnimation(int animation);
	double					getCurrentAnimationTotalTime();
	double					getCurrentAnimationCurrentTime();
	void					setCurrentAnimationCurrentTime(double d);
	bool					isAnimationLoop() const;
	void					setAnimationLoop(bool);
	bool					isAnimationMoveNextOnEnd() const;
	void					setAnimationMoveNextOnEnd(bool);
	float					getAnimationSpeed() const;
	void					setAnimationSpeed(float);
	virtual size_t			getSubMeshCount(void);
	virtual shaderMaterial*	getSubMeshMaterial(size_t index);
	virtual std::string&	getSubMeshName(size_t index);
	virtual std::string&	getName(void);
	virtual std::string&	getTextureName(size_t index);
	virtual void			setTexture(size_t index, const std::string name);
};