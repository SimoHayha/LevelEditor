#include "FBX_Mesh.h"
#include <Windows.h>
#include "Dictionnary.h"

id::FBX::Mesh::Mesh(void) :
	controlPointActive(true),
	normalActive(false),
	UVActive(false),
	vertice(nullptr),
	indice(nullptr)
{
}

id::FBX::Mesh::~Mesh(void)
{
}

bool
id::FBX::Mesh::init(FbxMesh* inMesh)
{
	const FbxVector4*				controls = inMesh->GetControlPoints();

	node		= inMesh->GetNode();
	if (node == nullptr)
		return false;
	polyCount	= inMesh->GetPolygonCount();
	normalActive= inMesh->GetElementNormalCount() > 0;
	UVActive	= inMesh->GetElementUVCount() > 0;
	vertexCount	= inMesh->GetControlPointsCount();
	matIndex	= nullptr;
    matMappingMode = FbxGeometryElement::eNone;

	hasVertexCache = inMesh->GetDeformerCount(FbxDeformer::eVertexCache) &&
		(static_cast<FbxVertexCacheDeformer*>(inMesh->GetDeformer(0, FbxDeformer::eVertexCache)))->IsActive();
	hasShape = inMesh->GetShapeCount() > 0;
	hasSkin = inMesh->GetDeformerCount(FbxDeformer::eSkin) > 0;
	hasDeformation = hasVertexCache || hasShape || hasSkin;

	fillSubMesh(inMesh);
	setMapping(inMesh);
	if (controlPointActive == false)
	{
		vertexCount = polyCount * 3;
	}
	indexCount = polyCount * 3;
	try
	{
		vertice = new VertexType[vertexCount];
		indice = new unsigned long[indexCount];
	}
	catch (std::bad_alloc ba)
	{
		ba.what();
		return false;
	}
    FbxStringList lUVNames;
    inMesh->GetUVSetNames(lUVNames);
    if (UVActive && lUVNames.GetCount())
        UVName = lUVNames[0];
	if (controlPointActive)
	{
		if (loadByControlPoint(controls, inMesh) == false)
			return false;
	}
	loadByPolygon(controls, inMesh);
	loadMaterial();
	return true;
}

bool
id::FBX::Mesh::loadMaterial(void)
{
	int					size = subMeshes.Size();
	FbxSurfaceMaterial*	mat;
	Dictionnary*		dico = Dictionnary::getInstance();

	for (int i = 0; i < size; ++i)
	{
		mat = node->GetMaterial(i);
		if (mat)
		{
			subMeshes[i]->material = new (std::nothrow) id::FBX::Material;
			if (subMeshes[i]->material == nullptr)
				return false;
			if (!subMeshes[i]->material->init(mat))
				return false;
		}
	}
	return true;
}

bool
id::FBX::Mesh::fillSubMesh(FbxMesh* inMesh)
{
	int matCount;
    int offset;
	int Index;

	if (inMesh->GetElementMaterial())
    {
        matIndex = &inMesh->GetElementMaterial()->GetIndexArray();
        matMappingMode = inMesh->GetElementMaterial()->GetMappingMode();
        if (matIndex && matMappingMode == FbxGeometryElement::eByPolygon)
        {
            if (matIndex->GetCount() == polyCount)
            {
                for (size_t i = 0; i < polyCount; ++i)
				{
                    Index = matIndex->GetAt(i);
                    if (subMeshes.GetCount() < Index + 1)
                        subMeshes.Resize(Index + 1);
                    if (subMeshes[Index] == NULL)
                        subMeshes[Index] = new SubMesh;
                    subMeshes[Index]->triangleCount += 1;
                }
                for (int i = 0; i < subMeshes.GetCount(); i++)
                    if (subMeshes[i] == NULL)
                        subMeshes[i] = new SubMesh;
                matCount = subMeshes.GetCount();
                offset = 0;
                for (int lIndex = 0; lIndex < matCount; ++lIndex)
                {
                    subMeshes[lIndex]->indexOffset = offset;
                    offset += subMeshes[lIndex]->triangleCount * 3;
					subMeshes[lIndex]->triangleCount = 0;
                }
            }
        }
    }
	if (subMeshes.GetCount() == 0)
    {
        subMeshes.Resize(1);
        subMeshes[0] = new SubMesh();
    }
	return true;
}

bool
id::FBX::Mesh::loadByPolygon(const FbxVector4* controls, FbxMesh* inMesh)
{
	size_t		vertexIndex = 0;
	size_t		indexOffset;
	size_t		Index;
	FbxVector4	vertexVect;
	FbxVector4	normalVect;
	FbxVector2	UVvect;
	bool		map;
	int			controlsIndex;

	for (size_t i = 0; i < polyCount; ++i)
	{
		Index = 0;
		if (matIndex && matMappingMode == FbxGeometryElement::eByPolygon)
			Index = matIndex->GetAt(i);
		indexOffset = subMeshes[Index]->indexOffset + subMeshes[Index]->triangleCount * 3;
		for (size_t j = 0; j < 3; ++j)
		{
			controlsIndex = inMesh->GetPolygonVertex(i, j);
			if (controlPointActive)
				indice[indexOffset + j] = static_cast<unsigned long>(controlsIndex);
			else
			{
				indice[indexOffset + j] = static_cast<unsigned long>(vertexIndex);
				vertexVect = controls[controlsIndex];
				vertice[vertexIndex].vertex.x = static_cast<float>(vertexVect[0]);
				vertice[vertexIndex].vertex.y = static_cast<float>(vertexVect[1]);
				vertice[vertexIndex].vertex.z = static_cast<float>(vertexVect[2]);
				vertice[vertexIndex].vertex.w = 1;
				if (normalActive)
				{
					inMesh->GetPolygonVertexNormal(i, j, normalVect);
					vertice[vertexIndex].normal.x = static_cast<float>(normalVect[0]);
					vertice[vertexIndex].normal.y = static_cast<float>(normalVect[1]);
					vertice[vertexIndex].normal.z = static_cast<float>(normalVect[2]);
					vertice[vertexIndex].normal.w = 1;
				}
				if (UVActive)
				{
					if (inMesh->GetPolygonVertexUV(i, j, UVName.Buffer(), UVvect, map) == false)
						return false;
					if (!map)
					{
						vertice[vertexIndex].UV.x = static_cast<float>(UVvect[0]);
						vertice[vertexIndex].UV.y = 1 - static_cast<float>(UVvect[1]);
					}
					else
					{	
						vertice[vertexIndex].UV.x = 0.0f;
						vertice[vertexIndex].UV.y = 0.0f;
					}
				}
			}
			++vertexIndex;
		}
		subMeshes[Index]->triangleCount += 1;
	}
	return true;
}

bool
id::FBX::Mesh::loadByControlPoint(const FbxVector4* controls, FbxMesh* inMesh)
{
	FbxVector4						vertexVect;
	FbxVector4						normalVect;
	FbxVector2						UVvect;
	const FbxGeometryElementNormal*	normalEl = nullptr;
	const FbxGeometryElementUV*		UVEl = nullptr;
	size_t							normalIndex;
	size_t							UVIndex;

	if (normalActive)
		normalEl = inMesh->GetElementNormal();
	if (UVActive)
		UVEl = inMesh->GetElementUV();

	for (size_t i = 0; i < vertexCount; ++i)
	{
		vertexVect = controls[i];
		vertice[i].vertex.x = static_cast<float>(vertexVect[0]);
		vertice[i].vertex.y = static_cast<float>(vertexVect[1]);
		vertice[i].vertex.z = static_cast<float>(vertexVect[2]);
		vertice[i].vertex.w = 1;
		if (normalActive)
		{
			normalIndex = i;
			if (normalEl->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				normalIndex = normalEl->GetIndexArray().GetAt(i);
			normalVect = normalEl->GetDirectArray().GetAt(normalIndex);
			vertice[i].normal.x = static_cast<float>(normalVect[0]);
			vertice[i].normal.y = static_cast<float>(normalVect[1]);
			vertice[i].normal.z = static_cast<float>(normalVect[2]);	
			vertice[i].normal.w = 1;
		}
		if (UVActive)
		{
			UVIndex = i;
			if (UVEl->GetReferenceMode() == FbxLayerElement::eIndexToDirect)
				UVIndex = UVEl->GetIndexArray().GetAt(i);
			UVvect = UVEl->GetDirectArray().GetAt(UVIndex);
			vertice[i].UV.x = static_cast<float>(UVvect[0]);
			vertice[i].UV.y = static_cast<float>(UVvect[1]);
			
		}
	}
	return true;
}

void
id::FBX::Mesh::setMapping(FbxMesh* inMesh)
{
	FbxGeometryElement::EMappingMode normalMap = FbxGeometryElement::eNone;
	FbxGeometryElement::EMappingMode UVMap = FbxGeometryElement::eNone;

	if (normalActive)
	{
		normalMap = inMesh->GetElementNormal(0)->GetMappingMode();
		if (normalMap == FbxGeometry::eNone)
			normalActive = false;
		if (normalActive && normalMap != FbxGeometryElement::eByControlPoint)
			controlPointActive = false;
	}
	if (UVActive)
	{
		UVMap = inMesh->GetElementUV(0)->GetMappingMode();
		if (UVMap == FbxGeometry::eNone)
			UVActive = false;
		if (normalActive && UVMap != FbxGeometryElement::eByControlPoint)
			controlPointActive = false;
	}
}

void
id::FBX::Mesh::updateVertexPosition(FbxMesh* mesh, FbxVector4* vertices)
{
	float * lVertices = nullptr;
	FbxVector4	vertexVect;
	int lVertexCount = mesh->GetControlPointsCount();
    if (controlPointActive)
    {
		for (int i = 0; i < lVertexCount; ++i)
		{
			vertexVect = vertices[i];
			vertice[i].vertex.x = static_cast<float>(vertexVect[0]);
			vertice[i].vertex.y = static_cast<float>(vertexVect[1]);
			vertice[i].vertex.z = static_cast<float>(vertexVect[2]);
			vertice[i].vertex.w = 1;
		}
    }
    else
    {
		int vertexIndex = 0;
		for (size_t i = 0; i < polyCount; ++i)
		{
			for (size_t j = 0; j < 3; ++j)
			{
				int controlsIndex = mesh->GetPolygonVertex(i, j);
				vertexVect = vertices[controlsIndex];
				vertice[vertexIndex].vertex.x = static_cast<float>(vertexVect[0]);
				vertice[vertexIndex].vertex.y = static_cast<float>(vertexVect[1]);
				vertice[vertexIndex].vertex.z = static_cast<float>(vertexVect[2]);
				vertice[vertexIndex].vertex.w = 1;
				++vertexIndex;
			}
		}
     }
}

FbxArray<id::FBX::Mesh::SubMesh*>&
	id::FBX::Mesh::getSubMeshArray(void)
{
	return subMeshes;
}

bool
id::FBX::Mesh::getHasUV(void)
{
	return UVActive;
}