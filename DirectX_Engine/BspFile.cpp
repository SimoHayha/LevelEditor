#include <fstream>

#include "BspFile.h"

BspFile::BspFile() :
	faces(nullptr),
	vertex(nullptr),
	edges(nullptr),
	planes(nullptr),
	header(nullptr),
	surfedges(nullptr),
	vertices(nullptr),
	indices(nullptr)
{
}

BspFile::~BspFile()
{
}

bool	BspFile::load(std::string & filename)
{
	std::ifstream	file(filename, std::ios::in | std::ios::binary);

	if (file.is_open())
	{
		char	magic[4] = {0};
		try
		{
			this->header = new dheader_t;
		}
		catch (std::bad_alloc)
		{
			return false;
		}

		file.read((char *)this->header, HEADERSIZE);

		if (header->ident != IDBSPHEADER)
		{
			file.close();
			return false;
		}

		try
		{
			this->planes    = new dplane_t[header->lumps[LUMP_PLANES].filelen / PLANE_SIZE];
			this->vertex    = new dvertex_t[header->lumps[LUMP_VERTEXES].filelen / VERTEX_SIZE];
			this->edges     = new dedge_t[header->lumps[LUMP_EDGES].filelen / EDGE_SIZE];
			this->surfedges = new short[header->lumps[LUMP_SURFEDGES].filelen / SURFEDGES_SIZE];
			this->faces     = new dface_t[header->lumps[LUMP_FACES].filelen / FACE_SIZE];
		}
		catch (std::bad_alloc)
		{
			return false;
		}

		file.seekg(this->header->lumps[LUMP_PLANES].fileofs, std::ios::beg);
		file.read((char *)this->planes, this->header->lumps[LUMP_PLANES].filelen);

		file.seekg(this->header->lumps[LUMP_VERTEXES].fileofs, std::ios::beg);
		file.read((char *)this->vertex, this->header->lumps[LUMP_VERTEXES].filelen);

		file.seekg(this->header->lumps[LUMP_EDGES].fileofs, std::ios::beg);
		file.read((char *)this->edges, this->header->lumps[LUMP_EDGES].filelen);

		file.seekg(this->header->lumps[LUMP_SURFEDGES].fileofs, std::ios::beg);
		file.read((char *)this->surfedges, this->header->lumps[LUMP_SURFEDGES].filelen);

		file.seekg(this->header->lumps[LUMP_FACES].fileofs, std::ios::beg);
		file.read((char *)this->faces, this->header->lumps[LUMP_FACES].filelen);

		for (int i = 0; i < this->header->lumps[LUMP_VERTEXES].filelen / (int)VERTEX_SIZE; ++i)
		{
			try
			{
				this->stdVertices.push_back(DirectX::XMFLOAT3(this->vertex[i].point.x, this->vertex[i].point.y, this->vertex[i].point.z));
			}
			catch (std::bad_alloc)
			{
				return false;
			}
		}

		for (int i = 0; i < this->header->lumps[LUMP_EDGES].filelen / (int)EDGE_SIZE; ++i)
		{
			try
			{
				this->stdIndices.push_back(this->edges[i].v[0]);
				this->stdIndices.push_back(this->edges[i].v[1]);
			}
			catch (std::bad_alloc)
			{
				return false;
			}
		}
	}

	return true;
}

void	BspFile::clean()
{
	delete this->faces;
	delete this->vertex;
	delete this->edges;
	delete this->planes;
	delete this->header;
	delete this->surfedges;
	delete this->vertices;
	delete this->indices;

	this->stdIndices.clear();
	this->stdVertices.clear();
}

VertexType *	BspFile::getVertices()
{
	if (nullptr == this->vertices)
	{
		try
		{
			this->vertices = new VertexType[this->stdVertices.size()];
		}
		catch (std::bad_alloc)
		{
			return nullptr;
		}

		for (auto i = 0u; i < this->stdVertices.size(); ++i)
		{
			this->vertices[i].vertex.x = this->stdVertices[i].x;
			this->vertices[i].vertex.y = this->stdVertices[i].y;
			this->vertices[i].vertex.z = this->stdVertices[i].z;
			this->vertices[i].vertex.w = 1.0f;
			this->vertices[i].normal.x = 0.0f;
			this->vertices[i].normal.y = 0.0f;
			this->vertices[i].normal.z = 0.0f;
			this->vertices[i].normal.w = 0.0f;
			this->vertices[i].UV.x     = 0.0f;
			this->vertices[i].UV.y     = 0.0f;
		}
	}

	return this->vertices;
}

unsigned long *	BspFile::getIndices()
{
	if (nullptr == this->indices)
	{
		try
		{
			this->indices = new unsigned long[this->stdIndices.size()];
		}
		catch (std::bad_alloc)
		{
			return nullptr;
		}

		for (auto i = 0u; i < this->stdIndices.size(); ++i)
			this->indices[i] = this->stdIndices[i];
	}

	return this->indices;
}

int	BspFile::getVerticesCount()
{
	return this->stdVertices.size();
}

int	BspFile::getIndicesCount()
{
	return this->stdIndices.size();
}