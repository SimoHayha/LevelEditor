#include "FBX_Model.h"

id::FBX::Model::Model(void) : _bonesIndices(nullptr), _bonesVertices(nullptr), _hasTexture(true)
{
}


id::FBX::Model::~Model(void)
{
	delete _bonesIndices;
	delete _bonesVertices;
}

bool
id::FBX::Model::init(FbxScene* scene)
{
	FbxNode*			rootNode = scene->GetRootNode();
	size_t				numChild, numMesh;

	_scene = scene;
	if (rootNode)
	{
		numChild = rootNode->GetChildCount();
		for (size_t i = 0 ; i < numChild ; ++i)
			if (!initRec(rootNode->GetChild(i), -1))
				return false;
		numMesh = _meshes.size();
		for (size_t i = 0 ; i < numMesh ; ++i)
			if (_meshes[i].hasDeformation &&
				!_meshes[i].deformation.Init(_meshes[i].node, _meshes[i].node->GetMesh()))
				return false;
		return allocBones();
	}
	return false;
}

bool
id::FBX::Model::initRec(FbxNode* node, int parentIndex)
{
	FbxNodeAttribute*	attrib;
	size_t				numChild;
	int					me = -1;

	if (node == nullptr)
		return true;
	attrib = node->GetNodeAttribute();
	if (attrib != nullptr && attrib->GetAttributeType() == FbxNodeAttribute::eMesh)
	{
		try
		{
			_meshes.push_back(id::FBX::Mesh());
		}
		catch (std::bad_alloc ba)
		{
			ba.what();
			return false;
		}
		me = _meshes.size() - 1;
		if (_meshes[me].init(node->GetMesh()) == false)
			return false;
		if (_hasTexture)
			_hasTexture = _meshes[me].getHasUV();
	}
	else if (attrib != nullptr && attrib->GetAttributeType() == FbxNodeAttribute::eSkeleton) 
	{
		if (!initBone(node, parentIndex))
			return false;
		me = _bones.size() - 1;
	}
	numChild = node->GetChildCount();
	for (size_t i = 0 ; i < numChild ; ++i)
		if (!initRec(node->GetChild(i), me))
			return false;
	return true;
}

bool
id::FBX::Model::initBone(FbxNode* node, int parentIndex)
{
	FbxSkeleton*	bone = node->GetSkeleton();
	size_t			myBone;

	if (bone != nullptr)
	{
		try
		{
			_bones.push_back(Bone());
		}
		catch (std::bad_alloc b)
		{
			b.what();
			return false;
		}
		myBone = _bones.size() - 1;
		_bones[myBone].parentIndex = parentIndex;
		_bones[myBone].node = node;
	}
	return true;
}

bool
id::FBX::Model::allocBones(void)
{
	size_t	numBones = _bones.size();
	if (numBones <= 1)
		return true;
	_bonesVertices = new (std::nothrow) VertexType[numBones];
	if (_bonesVertices == nullptr)
		return false;
	_bonesIndices = new (std::nothrow) unsigned long[(numBones - 1) * 2];
	if (_bonesIndices == nullptr)
		return false;
	return true;
}

size_t
id::FBX::Model::getMeshCount(void)
{
	return _meshes.size();
}

size_t
id::FBX::Model::getBonesCount(void)
{
	return _bones.size();
}

id::FBX::Mesh&
id::FBX::Model::getMesh(size_t index)
{
	return _meshes[index % _meshes.size()];
}

id::FBX::Model::Bone&
id::FBX::Model::getBone(size_t index)
{
	return _bones[index % _bones.size()];
}

void	
id::FBX::Model::getBonesVerticesIndices(VertexType*& vertices,
									  unsigned long*& indices)
{
	vertices = _bonesVertices;
	indices = _bonesIndices;
}

FbxScene*
id::FBX::Model::getScene(void)
{
	return _scene;
}

bool
id::FBX::Model::getHasTexture(void)
{
	return _hasTexture;
}