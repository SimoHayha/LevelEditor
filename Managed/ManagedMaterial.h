#pragma once

#include "..\DirectX_Engine\UnmanagedMaterial.h"
#include "..\DirectX_Engine\UnmanagedEntity.h"

using namespace System;

public ref class ManagedMaterial
{
	UnmanagedEntity*	_entity;
	int					_index;

public:
	ManagedMaterial();
	~ManagedMaterial();

	void setEntityMat(UnmanagedEntity* entity, size_t index);

	UnmanagedMaterial*	_unmanaged;

	property String^	Name
	{
		String^ get()
		{
			return gcnew String(_entity->getMaterialName(_index).c_str());
		}
		void set(String^ v)
		{
			throw gcnew NotImplementedException();
		}
	}

	property String^	TextureName
	{
		String^ get()
		{
			return gcnew String(_entity->getTextureName(_index).c_str());
		}
		void set(String^ v)
		{
			std::string convert;
			using namespace Runtime::InteropServices;
			const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(v)).ToPointer();
			convert = chars;
			Marshal::FreeHGlobal(IntPtr((void*)chars));
			_entity->setTexture(_index, convert);
		}
	}

	property float	ColorX
	{
		float get()
		{
			return _unmanaged->getColorX();
		}
		void set(float v)
		{
			_unmanaged->setColorX(v);
		}
	}
	property float	ColorY
	{
		float get()
		{
			return _unmanaged->getColorY();
		}
		void set(float v)
		{
			_unmanaged->setColorY(v);
		}
	}
	property float	ColorZ
	{
		float get()
		{
			return _unmanaged->getColorZ();
		}
		void set(float v)
		{
			_unmanaged->setColorZ(v);
		}
	}

	property float	EmmisiveX
	{
		float get()
		{
			return _unmanaged->getEmmisiveX();
		}
		void set(float v)
		{
			_unmanaged->setEmmisiveX(v);
		}
	}
	property float	EmmisiveY
	{
		float get()
		{
			return _unmanaged->getEmmisiveY();
		}
		void set(float v)
		{
			_unmanaged->setEmmisiveY(v);
		}
	}
	property float	EmmisiveZ
	{
		float get()
		{
			return _unmanaged->getEmmisiveZ();
		}
		void set(float v)
		{
			_unmanaged->setEmmisiveZ(v);
		}
	}
	property float	AmbientX
	{
		float get()
		{
			return _unmanaged->getAmbientX();
		}
		void set(float v)
		{
			_unmanaged->setAmbientX(v);
		}
	}
	property float	AmbientY
	{
		float get()
		{
			return _unmanaged->getAmbientY();
		}
		void set(float v)
		{
			_unmanaged->setAmbientY(v);
		}
	}
	property float	AmbientZ
	{
		float get()
		{
			return _unmanaged->getAmbientZ();
		}
		void set(float v)
		{
			_unmanaged->setAmbientZ(v);
		}
	}
	property float	DiffuseX
	{
		float get()
		{
			return _unmanaged->getDiffuseX();
		}
		void set(float v)
		{
			_unmanaged->setDiffuseX(v);
		}
	}
	property float	DiffuseY
	{
		float get()
		{
			return _unmanaged->getDiffuseY();
		}
		void set(float v)
		{
			_unmanaged->setDiffuseY(v);
		}
	}
	property float	DiffuseZ
	{
		float get()
		{
			return _unmanaged->getDiffuseZ();
		}
		void set(float v)
		{
			_unmanaged->setDiffuseZ(v);
		}
	}
	property float	SpecularX
	{
		float get()
		{
			return _unmanaged->getSpecularX();
		}
		void set(float v)
		{
			_unmanaged->setSpecularX(v);
		}
	}
	property float	SpecularY
	{
		float get()
		{
			return _unmanaged->getSpecularY();
		}
		void set(float v)
		{
			_unmanaged->setSpecularY(v);
		}
	}
	property float	SpecularZ
	{
		float get()
		{
			return _unmanaged->getSpecularZ();
		}
		void set(float v)
		{
			_unmanaged->setSpecularZ(v);
		}
	}
	property float	Shinness
	{
		float get()
		{
			return _unmanaged->getShinness();
		}
		void set(float v)
		{
			_unmanaged->setShinness(v);
		}
	}
};