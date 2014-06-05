#pragma once

#include <vector>

#include <DirectXCollision.h>
#include <DirectXMath.h>
#include <d3d11.h>

#include "VertexType.h"
#include "Buffers.h"
#include "BoundingBox.h"
#include "UnmanagedEntity.h"

class ShaderManager;
class LuaScript;
class VirtualShader;
class Texture;

struct shaderMaterial
{
	DirectX::XMFLOAT4	color;
	DirectX::XMFLOAT4	emmisive;
	DirectX::XMFLOAT4	ambient;
	DirectX::XMFLOAT4	diffuse;
	DirectX::XMFLOAT4	specular;
	float				shinness;
	DirectX::XMFLOAT3	pad;
};

class Entity
{
public:
	enum Type
	{
		E_MODEL,
		E_SKELETON,
		E_GRID,
		E_LIGHT,
		E_CUBE,
		E_SPHERE,
		E_CAMERA,
		E_FIELD,
		E_OVERLAY,
		E_ARROW,
		E_CONTAINER,
		E_LINELIST
	};

	virtual							~Entity();
	int&							getType(void);
	virtual void					setShader(ShaderManager* shader);
	VirtualShader*					getShader(void);
	virtual int						getIndexCount(size_t) = 0;
	virtual size_t					getMeshCount(void) = 0;
	virtual	void					getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index) = 0;
	virtual bool					refresh(ID3D11DeviceContext* context, float dt);
	virtual void					render(ID3D11DeviceContext* context, size_t index, size_t offset = 0);
	virtual bool					render(ID3D11DeviceContext* context,
										   DirectX::XMMATRIX& viewMatrix,
										   DirectX::XMMATRIX& worldMatrix,
										   DirectX::XMMATRIX& projectionMatrix);
	void							translate(float x, float y, float z);
	void							scale(float x, float y, float z);
	void							rotate(float x, float y, float z);
	virtual bool					appendChild(Entity * child);
	bool							hideEntity(Entity * child);
	bool							removeEntity(Entity * child);
	bool							childRefresh(ID3D11DeviceContext * context, float dt, Entity * & bind);
	void							childRender(ID3D11DeviceContext * context, float dt,
												DirectX::XMMATRIX& viewMatrix,
												DirectX::XMMATRIX& worldMatrix,
					   							DirectX::XMMATRIX& projectionMatrix,
												std::vector<DirectX::XMMATRIX*>& parentMatrices);
	size_t							getChildCount(void);
	Entity*							getChild(size_t index);
	virtual size_t					getSubMeshCount(void);
	virtual shaderMaterial*			getSubMeshMaterial(size_t index);
	virtual std::string&			getSubMeshName(size_t index);
	virtual std::string&			getTextureName(size_t index);
	virtual void					setTexture(size_t index, const std::string name);
	virtual std::string&			getScriptName(void);
	virtual void					setScript(const std::string name);
	virtual std::string&			getName(void);
	std::vector<Entity*>&			getChilds(void);
	void							getTransformMatrix(DirectX::XMMATRIX& transform);
	virtual void					getGlobalTransformMatrix(DirectX::XMMATRIX& transform);
	std::string&					getFilePath(void);
	bool							setFilePath(const std::string& path);
	void							setUnmanaged(UnmanagedEntity * unmanaged);
	void							setScript(LuaScript* script);
	bool							makeAabb(VertexType * vertices, int count);
	bool							makeEmptyAabb();
	bool							mergeAabb(DirectX::BoundingBox & that);
	DirectX::BoundingBox &			getAabb();
	bool							hasAabb();
	void							select();
	void							unselect();
	virtual void					onFeedback();
	void							getRight(DirectX::XMVECTOR & right);
	void							getUp(DirectX::XMVECTOR & up);
	void							getForward(DirectX::XMVECTOR & forward);
	UnmanagedEntity::Transform &	getTransform();
	void							setFather(Entity * father);

public:
	enum ItemType
	{
		E_TRANSX,
		E_TRANSY,
		E_TRANSZ,
		E_ROTATEX,
		E_ROTATEY,
		E_ROTATEZ,
		E_SCALEX,
		E_SCALEY,
		E_SCALEZ
	};

	template <typename T>
	T		getItem(ItemType);
	template <typename T>
	void	setItem(ItemType, T value);

	float					getTransX() const;
	float					getTransY() const;
	float					getTransZ() const;
	float					getRotX() const;
	float					getRotY() const;
	float					getRotZ() const;
	float					getScaleX() const;
	float					getScaleY() const;
	float					getScaleZ() const;
	void					setTransX(float);
	void					setTransY(float);
	void					setTransZ(float);
	void					setRotateX(float);
	void					setRotateY(float);
	void					setRotateZ(float);
	void					setScaleX(float);
	void					setScaleY(float);
	void					setScaleZ(float);
	void					setName(const std::string &);
	Entity *				getFather() const;
	DirectX::XMFLOAT3		getBoundingBoxCenter() const;
	DirectX::XMFLOAT3		getBoundingBoxExtend() const;
	bool					isSelected() const;
	void					setSelected(bool b);
	bool					isScriptRunning() const;
	void					setScriptRunning(bool b);

private:
	enum Feedback
	{
		E_FEEDBACK_EMPTY			= 1,
		E_FEEDBACK_TRANSX			= 2,
		E_FEEDBACK_TRANSY			= 4,
		E_FEEDBACK_TRANSZ			= 8,
		E_FEEDBACK_ROTX				= 16,
		E_FEEDBACK_ROTY				= 32,
		E_FEEDBACK_ROTZ				= 64,
		E_FEEDBACK_SCALEX			= 128,
		E_FEEDBACK_SCALEY			= 256,
		E_FEEDBACK_SCALEZ			= 512,
		E_FEEDBACK_SCRIPT_RUNNING	= 1024
	};

	int		feedback;
	
	void	onProcessFeedback();

protected:
	Entity(void);

	UnmanagedEntity::Transform	_transform;
	VirtualShader*				myShader;
	std::vector<Buffers*>		_buffers;
	size_t						_stride;
	D3D_PRIMITIVE_TOPOLOGY		_topology;
	int							_type;
	std::string					_filePath;
	LuaScript*					_script;
	bool						_scriptRunning;
	bool						_selected;
	UnmanagedEntity *			_unmanaged;
	Entity *					_father;
	std::vector<Entity *>		_childs;
	shaderMaterial				_defaultMat;
	std::string					_defaultMatName;
	Texture*					_texture;
	std::string					_textureName;
	std::string					_scriptName;
	std::string					_name;
	BoundingBox					_boundingBox;
	DirectX::XMFLOAT3			_right;
	DirectX::XMFLOAT3			_up;
	DirectX::XMFLOAT3			_forward;

private:
	static const int MAX_TRANS_X = 10000;
	static const int MIN_TRANS_X = -10000;
	static const int MAX_TRANS_Y = 10000;
	static const int MIN_TRANS_Y = -10000;
	static const int MAX_TRANS_Z = 10000;
	static const int MIN_TRANS_Z = -10000;
	static const int MAX_SCALE_X = 100;
	static const int MIN_SCALE_X = -100;
	static const int MAX_SCALE_Y = 100;
	static const int MIN_SCALE_Y = -100;
	static const int MAX_SCALE_Z = 100;
	static const int MIN_SCALE_Z = -100;
};


typedef Entity::ItemType	DATA;

#include "Entity.hpp"