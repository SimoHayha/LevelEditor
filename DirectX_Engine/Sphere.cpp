#include "Sphere.h"
#include "Dictionnary.h"
#include "VirtualShader.h"

Sphere::Sphere(void) :
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	_type = Entity::E_SPHERE;
}
bool	Sphere::init(ID3D11Device* device)
{
	Dictionnary*	dico;
	Buffers*		buffer;
	if ((dico = dico->getInstance()) == nullptr)
		return false;
	buffer = dico->getBuffer("__sphere", 0);
	if (buffer == nullptr)
		return false;
	int rings = 500;
	int segment = 500;
	vertexCount = (rings + 1) * (segment + 1);
	indexCount = 2 * rings * (segment + 1);

	_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	_stride = sizeof(VertexType);
	try
	{
		_defaultMatName = "defaultMatSphere";
		_name = "sphere";
		_buffers.push_back(buffer);
	}
	catch (std::bad_alloc)
	{
		return false;
	}

	return (true);
}
int		Sphere::getIndexCount(size_t i)
{
	(void)i;
	return (this->indexCount) - 2;
}
size_t Sphere::getMeshCount(void)
{
	return 1;
}
void	Sphere::setVertices(float positionX, float positionY, float positionZ, VertexType* vertices,
							WORD* index, float normalX, float normalY, float normalZ, float tu, float tv)
{
	DirectX::XMVECTOR	normal = DirectX::XMLoadFloat4(&DirectX::XMFLOAT4(normalX, normalY, normalZ, 1.0f));

	DirectX::XMVector3Normalize(normal);  
	vertices[*index].vertex = DirectX::XMFLOAT4(positionX, positionY, positionZ, 1.0f);
	DirectX::XMStoreFloat4(&vertices[*index].normal, normal);
	vertices[*index].UV = DirectX::XMFLOAT2(tu, tv);
	++*index;
}

bool	Sphere::initializeBuffers(ID3D11Device* device)
{
	Dictionnary*		dico;
	Buffers*			buffer;
	VertexType*			vertices;
	unsigned long*		indices;
	unsigned long*		indicesIt;
	WORD				index = 0;	
	FLOAT				deltaRingAngle;
	FLOAT				deltaSegAngle;
	int					rings = 20;
	int					segment = 20;
	int					vertexCount;
	int					indexCount;
	int					currentRing;
	int					currentSegment;

	vertexCount = (rings + 1) * (segment + 1);
	indexCount = 2 * rings * (segment + 1);
	if ((dico = Dictionnary::getInstance()) == nullptr)
		return false;
	buffer = new Buffers();
	if (buffer == nullptr)
		return false;
	vertices = new (std::nothrow) VertexType[vertexCount];
	if (vertices == nullptr)
	{
		delete buffer;
		return (false);
	}
	indices = new (std::nothrow) unsigned long[indexCount];
	if (indices == nullptr)
	{
		delete buffer;
		delete []vertices;
		return (false);
	}
	indicesIt = indices;
	
	deltaRingAngle = (DirectX::XM_PI / rings);
	deltaSegAngle = (2.0f * DirectX::XM_PI / segment);

	for(currentRing = 0; currentRing < rings + 1; currentRing++)
	{
		FLOAT r0 = sinf(currentRing * deltaRingAngle);
		FLOAT y0 = cosf(currentRing * deltaRingAngle);

		for(currentSegment = 0; currentSegment < segment + 1; currentSegment++)
		{
			FLOAT x0 = r0 * sinf(currentSegment * deltaSegAngle);
			FLOAT z0 = r0 * cosf(currentSegment * deltaSegAngle);
			
			if(currentRing != rings)    
            {   
                *indicesIt = index;   
                ++indicesIt;
                *indicesIt = index + (WORD)(segment + 1);    
                ++indicesIt;    
            }
			setVertices(x0, y0, z0, vertices, &index, x0, y0, z0, 1.0f - ((FLOAT)currentSegment / (FLOAT)segment),
						(FLOAT)currentRing / (FLOAT)rings);
		}
	}
	if (!buffer->init(device, D3D11_USAGE_DEFAULT, sizeof(VertexType)* vertexCount, vertices,
					 sizeof(unsigned long) * indexCount, indices))
	{
		delete buffer;
		delete[] vertices;
		delete[] indices;
	}

	if (false == this->makeAabb(vertices, vertexCount))
	{
		delete buffer;
		delete[] vertices;
		delete[] indices;
		return false;
	}
	this->_boundingBox.createBBBuffers(DirectX::XMFLOAT3(this->_boundingBox.getAabb().Center.x, this->_boundingBox.getAabb().Center.y, this->_boundingBox.getAabb().Center.z), this->_boundingBox.getAabb().Extents.x * 2, this->_boundingBox.getAabb().Extents.y, this->_boundingBox.getAabb().Extents.z * 2, device);

	if (dico->getBuffer("__sphere", 0, buffer) == nullptr)
	{
		delete []vertices;
		delete []indices;
		delete buffer;
		return false;
	}

	delete []vertices;
	delete []indices;
	return true;
}

void     Sphere::getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index)
{
	(void)dt;
	(void)index;
	(void)newMatrix;
}