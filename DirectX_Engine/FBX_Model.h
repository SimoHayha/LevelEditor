#pragma once

#include <vector>
#include <d3d11.h>
#include <DirectXMath.h>
#include <fbxsdk.h>
#include "FBX_Mesh.h"

namespace id
{
	namespace FBX
	{
		class Model
		{
		public:
			struct Bone
			{
				int				parentIndex;
				FbxNode*		node;
			};
			Model(void);
			~Model(void);

			bool				init(FbxScene* scene);
			size_t				getMeshCount(void);
			size_t				getBonesCount(void);
			id::FBX::Mesh&		getMesh(size_t index);
			Bone&				getBone(size_t index);
			FbxScene*			getScene(void);
			bool				getHasTexture(void);
			void				getBonesVerticesIndices(VertexType*& vertices,
														unsigned long*& indices);

		private:
			std::vector<id::FBX::Mesh>	_meshes;
			std::vector<Bone>			_bones;
			FbxScene*					_scene;
			VertexType*					_bonesVertices;
			unsigned long*				_bonesIndices;
			bool						_hasTexture;

			bool	initRec(FbxNode* node, int parentParent);
			bool	initBone(FbxNode* node, int parentIndex);
			bool	allocBones(void);
		};
	}
}
