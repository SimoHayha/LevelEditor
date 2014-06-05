#pragma once

#ifdef UNMANAGED_EXPORTS
#define UNMANAGED_API __declspec(dllexport)
#else
#define UNMANAGED_API __declspec(dllimport)
#endif

struct shaderMaterial;
class UnmanagedEntity;

class UNMANAGED_API UnmanagedMaterial
{
	public:
		shaderMaterial* mat;

		UnmanagedMaterial();
		float getColorX(void);
		float getColorY(void);
		float getColorZ(void);
		float getEmmisiveX(void);
		float getEmmisiveY(void);
		float getEmmisiveZ(void);
		float getAmbientX(void);
		float getAmbientY(void);
		float getAmbientZ(void);
		float getDiffuseX(void);
		float getDiffuseY(void);
		float getDiffuseZ(void);
		float getSpecularX(void);
		float getSpecularY(void);
		float getSpecularZ(void);
		float getShinness(void);
		void setColorX(float v);
		void setColorY(float v); 
		void setColorZ(float v); 
		void setEmmisiveX(float v);
		void setEmmisiveY(float v); 
		void setEmmisiveZ(float v); 
		void setAmbientX(float v);
		void setAmbientY(float v);
		void setAmbientZ(float v);
		void setDiffuseX(float v);
		void setDiffuseY(float v);
		void setDiffuseZ(float v);
		void setSpecularX(float v); 
		void setSpecularY(float v); 
		void setSpecularZ(float v); 
		void setShinness(float v);

		void setEntityMat(UnmanagedEntity* entity, size_t index);
};
