#include "UnmanagedMaterial.h"
#include "Entity.h"

UnmanagedMaterial::UnmanagedMaterial() : mat (nullptr)
{
}

float UnmanagedMaterial::getColorX(void) { return mat->color.x; }
float UnmanagedMaterial::getColorY(void) { return mat->color.y; }
float UnmanagedMaterial::getColorZ(void) { return mat->color.z; }
float UnmanagedMaterial::getEmmisiveX(void) { return mat->emmisive.x; }
float UnmanagedMaterial::getEmmisiveY(void) { return mat->emmisive.y; }
float UnmanagedMaterial::getEmmisiveZ(void) { return mat->emmisive.z; }
float UnmanagedMaterial::getAmbientX(void) { return mat->ambient.x; }
float UnmanagedMaterial::getAmbientY(void) { return mat->ambient.y; }
float UnmanagedMaterial::getAmbientZ(void) { return mat->ambient.z; }
float UnmanagedMaterial::getDiffuseX(void) { return mat->diffuse.x; }
float UnmanagedMaterial::getDiffuseY(void) { return mat->diffuse.y; }
float UnmanagedMaterial::getDiffuseZ(void) { return mat->diffuse.z; }
float UnmanagedMaterial::getSpecularX(void) { return mat->specular.x; }
float UnmanagedMaterial::getSpecularY(void) { return mat->specular.y; }
float UnmanagedMaterial::getSpecularZ(void) { return mat->specular.z; }
float UnmanagedMaterial::getShinness(void) { return mat->shinness; }
void UnmanagedMaterial::setColorX(float v) { mat->color.x = v; }
void UnmanagedMaterial::setColorY(float v) { mat->color.y = v; }
void UnmanagedMaterial::setColorZ(float v) { mat->color.z = v; }
void UnmanagedMaterial::setEmmisiveX(float v) { mat->emmisive.x = v; }
void UnmanagedMaterial::setEmmisiveY(float v) { mat->emmisive.y = v; }
void UnmanagedMaterial::setEmmisiveZ(float v) { mat->emmisive.z = v; }
void UnmanagedMaterial::setAmbientX(float v) { mat->ambient.x = v; }
void UnmanagedMaterial::setAmbientY(float v) { mat->ambient.y = v; }
void UnmanagedMaterial::setAmbientZ(float v) { mat->ambient.z = v; }
void UnmanagedMaterial::setDiffuseX(float v) { mat->diffuse.x = v; }
void UnmanagedMaterial::setDiffuseY(float v) { mat->diffuse.y = v; }
void UnmanagedMaterial::setDiffuseZ(float v) { mat->diffuse.z = v; }
void UnmanagedMaterial::setSpecularX(float v) { mat->specular.x = v; }
void UnmanagedMaterial::setSpecularY(float v) { mat->specular.y = v; }
void UnmanagedMaterial::setSpecularZ(float v) { mat->specular.z = v; }
void UnmanagedMaterial::setShinness(float v) { mat->shinness = v; }

void UnmanagedMaterial::setEntityMat(UnmanagedEntity* entity, size_t index)
{
	mat = entity->getEntity()->getSubMeshMaterial(index);
}