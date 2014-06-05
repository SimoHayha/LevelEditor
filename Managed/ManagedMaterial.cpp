#include "stdafx.h"
#include "ManagedMaterial.h"


ManagedMaterial::ManagedMaterial()
{
	_unmanaged = new UnmanagedMaterial;
}

ManagedMaterial::~ManagedMaterial(void)
{
	delete _unmanaged;
}

void
ManagedMaterial::setEntityMat(UnmanagedEntity* entity, size_t index)
{
	_entity = entity;
	_index = index;
	_unmanaged->setEntityMat(entity, index);
}