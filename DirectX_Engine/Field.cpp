#include "Field.h"
#include "Dictionnary.h"
#include "VirtualShader.h"
#include "math.h"
#include <ctime>

Field::Field() :
	vertexBuffer(nullptr),
	indexBuffer(nullptr)
{
	unSteps = 1000;
	unSize = (2 << unSteps) + 1;
	fVariability = 0.1f; 
	fSpacing = 0.3f;
	srand(static_cast<unsigned int>(time(nullptr)));
	_type = Entity::E_FIELD;
}
float	Field::randomize()
{
	const int limit = 1000;
	return static_cast<float>((rand()%limit))/limit;
}

bool	Field::init(ID3D11Device* device)
{

	Dictionnary* dico;
	Buffers*	buffer;
	
	
	if ((dico = dico->getInstance()) == nullptr)
		return false;
	buffer = dico->getBuffer("__field", 0);
	if (buffer == nullptr)
		return false;
	_stride = sizeof(VertexType);
	_topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	try
	{
		_defaultMatName = "defaultMatField";
		_name = "field";
		_buffers.push_back(buffer);
	}
	catch (std::bad_alloc)
	{
		return false;
	}
	return (true);
}
void	Field::initialize()
{
	 vectPoints.resize(unSize);
	 for(unsigned int unI=0; unI<unSize; unI++)
	 {
		 vectPoints[unI].resize(unSize);
	 }
}
float	Field::DiamondStep(unsigned int unI, unsigned int unJ, unsigned int unHalfSpacing) 
{ 
	
  float sum = 0.0; 
  int n = 0; 
  
  if ((unI >= unHalfSpacing)  && 
	  (unJ >= unHalfSpacing)) 
	{ 
	  sum += vectPoints[unI-unHalfSpacing][unJ-unHalfSpacing]; 
	  n++; 
	} 
	 
  if ((unI >= unHalfSpacing)  && 
	  (unJ + unHalfSpacing < unSize)) 
	{ 
	  sum += vectPoints[unI-unHalfSpacing][unJ+unHalfSpacing];        
	  n++; 
	} 
	 
  if ((unI + unHalfSpacing <  unSize)  && 
	  (unJ >= unHalfSpacing)) 
	{ 
	  sum += vectPoints[unI+unHalfSpacing][unJ-unHalfSpacing]; 
	  n++; 
	} 
	 
  if ((unI + unHalfSpacing <  unSize)  && 
	  (unJ + unHalfSpacing <  unSize)) 
	{ 
	  sum += vectPoints[unI+unHalfSpacing][unJ+unHalfSpacing]; 
	  n++; 
	} 
	 
  return sum / n; 
} 

float	Field::SquareStep(unsigned int unI, unsigned int unJ, unsigned int unHalfSpacing) 
{ 
  float sum = 0.0; 
  int n = 0; 
	 
  if (unI >= unHalfSpacing) 
	{ 
	  sum += vectPoints[unI-unHalfSpacing][unJ]; 
	  n++; 
	} 
 
  if (unI + unHalfSpacing < unSize) 
	{ 
	  sum += vectPoints[unI+unHalfSpacing][unJ]; 
	  n++; 
	} 
		 
  if (unJ >= unHalfSpacing) 
	{ 
	  sum += vectPoints[unI][unJ-unHalfSpacing]; 
	  n++; 
	} 
 
  if (unJ + unHalfSpacing < unSize) 
	{ 
	  sum += vectPoints[unI][unJ+unHalfSpacing]; 
	  n++; 
	} 
	 
  return sum / n; 
} 

int		Field::getIndexCount(size_t)
{
	return (unSize-1) * (unSize-1) * 12;
}

size_t	Field::getMeshCount(void)
 {
	 return 1;
 }

void	Field::setVertices(float positionX, float positionY, float positionZ, int i, int j, VertexType* vertices,
						  unsigned long* indices, int* index, DirectX::XMVECTOR& normal, float tu, float tv)
{
	vertices[*index].vertex = DirectX::XMFLOAT4(positionX, positionY, positionZ, 1.0f);
	DirectX::XMStoreFloat4(&vertices[*index].normal, normal);
	if(positionY >= 55)
		_defaultMat.color = DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	else if(positionY >= 45)
		_defaultMat.color = DirectX::XMFLOAT4(0.6f, 0.2f, 0.0f, 0.0f);
	else if(positionY >= 25)
		_defaultMat.color = DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
	else if(positionY >= 0)
		_defaultMat.color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 0.0f);
	vertices[*index].UV = DirectX::XMFLOAT2(tu, tv);
	indices[*index] = *index;
	++*index;
}
void Field::Generate(float fLeftBottom, float fLeftTop, 
					 float fRightTop, float fRightBottom) 
{ 
   initialize(); 
 
  float				fMax;
  float				fMin;
  vectPoints[0][0] = fLeftBottom; 
  vectPoints[0][unSize-1] = fRightBottom; 
  vectPoints[unSize-1][0] = fLeftTop; 
  vectPoints[unSize-1][unSize-1] = fRightTop; 
 

  unsigned int unSpacing = unSize - 1;
	
  while (unSpacing > 1) 
	{ 
 
	  int unHalfSpacing = unSpacing / 2;
	   
	  for (unsigned int unI = unHalfSpacing; unI < unSize; unI += unSpacing) 
		{ 
		  for (unsigned int unJ = unHalfSpacing; unJ < unSize ; unJ += unSpacing ) 
			{ 
			  vectPoints[unI][unJ] = DiamondStep(unI, unJ, unHalfSpacing ) + randomize() * unSpacing * fVariability; 
			} 
		} 
 
	  for (unsigned int unI = 0; unI < unSize; unI += unHalfSpacing) 
		{ 
		  unsigned int unJStart = ((unI/unHalfSpacing) % 2 == 0 ) ? unHalfSpacing : 0; 
		   
		  for (unsigned int unJ = unJStart; unJ < unSize ; unJ += unSpacing ) 
			{ 
			  vectPoints[unI][unJ] = SquareStep(unI, unJ, unHalfSpacing ) + randomize() * unSpacing *  fVariability;
			} 
		} 
 
	  unSpacing = unHalfSpacing;
 
	} 

  fMax = vectPoints[0][0];
  fMin = vectPoints[0][0];
  for (unsigned int unI = 0; unI < (unSize-1); unI++ ) 
	for (unsigned int unJ = 0; unJ < (unSize-1); unJ++ ) 
      {
		if(vectPoints[unI][unJ] > fMax)
		  {
			fMax = vectPoints[unI][unJ];
		  }
		if(vectPoints[unI][unJ] < fMin)
		  {
			fMin = vectPoints[unI][unJ];
		  }
      }
} 

bool	Field::initializeBuffers(ID3D11Device* device)
{
	Dictionnary*		dico;
	VertexType*			vertices;
	Buffers*			buffer;
	unsigned long*		indices;
	int					index;	
	int					vertexCount;
	int					indexCount;

	float				fMax = fSpacing * (unSize/2);
	float				fMin = -fMax;
	float				fDiff;
	bool				filled;
	filled = true;
	fDiff = fMax - fMin;
	Generate();
	
	vertexCount = (unSize-1) * (unSize-1) * 20;
	indexCount = vertexCount;

	if ((dico = Dictionnary::getInstance()) == nullptr)
		return false;
	vertices = new VertexType[vertexCount];
	if (vertices == false)
		return (false);
	indices = new unsigned long[indexCount];
	if (indices == false)
	{
		delete []vertices;
		return (false);
	}
	buffer = new Buffers();
	if (buffer == nullptr)
	{
		delete []vertices;
		delete []indices;
		return (false);
	}

	index = 0;
	DirectX::XMVECTOR	vec1;
	DirectX::XMVECTOR	vec2;
	DirectX::XMVECTOR	normal;

	for(unsigned int i = 0; i < (unSize - 1); i++)
	{
		float	fZ = fMin + i * fSpacing; 
		for(unsigned int j = 0; j < (unSize - 1); j++)
		{
			float fX = fMin + j * fSpacing;
			if(filled)
			{
				DirectX::XMFLOAT3	A(fX, vectPoints[i][j], fZ);
				DirectX::XMFLOAT3	B(fX, vectPoints[i + 1][j], fZ + fSpacing);
				DirectX::XMFLOAT3	C(fX + fSpacing, vectPoints[i + 1][j + 1], fZ + fSpacing);
				DirectX::XMFLOAT3	D(fX + fSpacing, vectPoints[i][j + 1], fZ);

				vec1 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(B.x - A.x, B.y - A.y, B.z - A.z));
				vec2 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(C.x - A.x, C.y - A.y, C.z - A.z));
				normal = DirectX::XMVector3Cross(vec1, vec2);
			setVertices(fX, vectPoints[i][j], fZ, i, j, vertices, indices, &index, normal, (fX + fMax) / fMax * 2, (fZ + fMax) / fMax * 2);
			setVertices(fX, vectPoints[i + 1][j], fZ + fSpacing, i, j, vertices, indices, &index, normal, (fX + fMax) / fMax * 2, (fZ + fSpacing + fMax) / fMax * 2);
			setVertices(fX + fSpacing, vectPoints[i + 1][j + 1], fZ + fSpacing, i, j, vertices, indices, &index, normal, (fX + fSpacing + fMax) / fMax * 2, (fZ + fSpacing + fMax) / fMax * 2);

				/*vec1 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(C.x - B.x, C.y - B.y, C.z - B.z));
				vec2 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(D.x - B.x, D.y - B.y, D.z - B.z));
				normal = DirectX::XMVector3Cross(vec1, vec2);
			setVertices(fX, vectPoints[i + 1][j], fZ + fSpacing, i, j, vertices, indices, &index, normal, (fX + fMax) / fMax * 2, (fZ + fSpacing + fMax) / fMax * 2);
			setVertices(fX + fSpacing, vectPoints[i + 1][j + 1], fZ + fSpacing, i, j, vertices, indices, &index, normal, (fX + fSpacing + fMax) / fMax * 2, (fZ + fSpacing + fMax) / fMax * 2);
			setVertices(fX + fSpacing, vectPoints[i][j + 1], fZ, i, j, vertices, indices, &index, normal, (fX + fSpacing + fMax) / fMax * 2, (fZ + fMax) / fMax * 2);
*/
				vec1 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(D.x - C.x, D.y - C.y, D.z - C.z));
				vec2 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(A.x - C.x, A.y - C.y, A.z - C.z));
				normal = DirectX::XMVector3Cross(vec1, vec2);
			setVertices(fX + fSpacing, vectPoints[i + 1][j + 1], fZ + fSpacing, i, j, vertices, indices, &index, normal, (fX + fSpacing + fMax) / fMax * 2, (fZ +fSpacing + fMax) / fMax * 2);
			setVertices(fX + fSpacing, vectPoints[i][j + 1], fZ, i, j, vertices, indices, &index, normal, (fX + fSpacing + fMax) / fMax * 2, (fZ + fMax) / fMax * 2);
			setVertices(fX, vectPoints[i][j], fZ, i, j, vertices, indices, &index, normal, (fX + fMax) / fMax * 2, (fZ + fMax) / fMax * 2);

				/*vec1 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(A.x - D.x, A.y - D.y, A.z - D.z));
				vec2 = DirectX::XMLoadFloat3(&DirectX::XMFLOAT3(B.x - D.x, B.y - D.y, B.z - D.z));
				normal = DirectX::XMVector3Cross(vec1, vec2);
			setVertices(fX + fSpacing, vectPoints[i][j + 1], fZ, i, j, vertices, indices, &index, normal, (fX + fSpacing + fMax) / fMax * 2, (fZ + fMax) / fMax * 2);
			setVertices(fX, vectPoints[i][j], fZ, i, j, vertices, indices, &index, normal, (fX + fMax) / fMax * 2, (fZ + fMax) / fMax * 2);
			setVertices(fX, vectPoints[i + 1][j], fZ + fSpacing, i, j, vertices, indices, &index, normal, (fX  + fMax) / fMax * 2, (fZ + fSpacing + fMax) / fMax * 2);
			*/


			
			
			/*setVertices(fX, vectPoints[i + 1][j], fZ + fSpacing, i, j, vertices, indices, &index);
			setVertices(fX, vectPoints[i + 1][j], fZ + fSpacing, i, j, vertices, indices, &index);
			setVertices(fX, vectPoints[i][j + 1], fZ, i, j, vertices, indices, &index);
			setVertices(fX, vectPoints[i + 1][j], fZ + fSpacing, i, j, vertices, indices, &index);*/
			
		
			


			}
		}
	}


	if (!buffer->init(device, D3D11_USAGE_DEFAULT, sizeof(VertexType) * vertexCount,
					  vertices, sizeof(unsigned long) * indexCount, indices))
	{
		delete []vertices;
		delete []indices;
		delete buffer;
		return false;
	}
	if (dico->getBuffer("__field", 0, buffer) == nullptr)
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

void
Field::getWorldMatrix(DirectX::XMMATRIX& newMatrix, float dt, size_t index)
{
	(void)dt;
	(void)index;
	(void)newMatrix;
}