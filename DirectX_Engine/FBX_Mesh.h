#pragma once

#include <fbxsdk.h>
#include <DirectXMath.h>

#include "FBX_Material.h"

namespace id
{
	namespace FBX
	{
		struct Mesh
		{
			//struct VertexType
			//{
			//	DirectX::XMFLOAT4	vertex;
			//	DirectX::XMFLOAT4	normal;
			//	DirectX::XMFLOAT2	UV;
			//};

			struct SubMesh
			{
				SubMesh() : indexOffset(0), triangleCount(0), material(nullptr) {}
				int			indexOffset;
				int			triangleCount;
				Material*	material;
			};
			bool	controlPointActive;
			bool	normalActive;
			bool	UVActive;
			size_t	polyCount;
			size_t	vertexCount;
			size_t	indexCount;
			FbxString UVName;

			VertexType*							vertice;
			unsigned long*						indice;
			FbxNode*							node;
			FbxArray<SubMesh*>					subMeshes;
			FbxGeometryElement::EMappingMode	matMappingMode;
			FbxLayerElementArrayTemplate<int>*	matIndex;
			bool								hasVertexCache;
			bool								hasShape;
			bool								hasSkin;
			bool								hasDeformation;
			FbxDeformationsEvaluator			deformation;

								Mesh(void);
								~Mesh(void);
			bool				init(FbxMesh* inMesh);
			void				updateVertexPosition(FbxMesh* mesh, FbxVector4* vertices);
			FbxArray<SubMesh*>&	getSubMeshArray(void);
			bool				getHasUV(void);


		private:
			void				setMapping(FbxMesh* inMesh);
			bool				loadByControlPoint(const FbxVector4* controls, FbxMesh* inMesh);
			bool				loadByPolygon(const FbxVector4* controls, FbxMesh* inMesh);
			bool				loadMaterial(void);
			bool				fillSubMesh(FbxMesh* inMesh);
		};
	}
}