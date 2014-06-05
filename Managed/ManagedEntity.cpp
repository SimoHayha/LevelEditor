#include "stdafx.h"

#include "ManagedEntity.h"
#include "FeedbackEntity.h"


namespace ManagedLib
{
	ManagedEntity::ManagedEntity()
	{
		_feedback = new FeedbackEntity(this);
		_unmanaged = new UnmanagedEntity(_feedback);
		_childs = gcnew System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>();
		_viewPortID = 0;
	}

	ManagedEntity::!ManagedEntity()
	{
		delete _unmanaged;
		delete _feedback;
	}

	ManagedEntity::~ManagedEntity()
	{
	}

	void ManagedEntity::marshalString(String^ s, std::string& os)
	{
		using namespace Runtime::InteropServices;
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

	void
	ManagedEntity::raiseValueChanged(int i)
	{
		if (_valueChanged != nullptr)
			_valueChanged->Invoke(this);
	}

	void
	ManagedEntity::raiseTotalTimeChanged(double time)
	{
		OnPropertyChanged("TotalTime");
	}

	void
	ManagedEntity::raiseCurrentTimeChanged(double time)
	{
		OnPropertyChanged("CurrentTime");
	}

	void ManagedEntity::raiseCameraMoved()
	{
		OnPropertyChanged("RotX");
		OnPropertyChanged("RotY");
		OnPropertyChanged("RotZ");
		OnPropertyChanged("TransX");
		OnPropertyChanged("TransY");
		OnPropertyChanged("TransZ");
	}

	void ManagedEntity::raiseTransformationUpdate()
	{
		OnPropertyChanged("RotX");
		OnPropertyChanged("RotY");
		OnPropertyChanged("RotZ");
		OnPropertyChanged("TransX");
		OnPropertyChanged("TransY");
		OnPropertyChanged("TransZ");
		OnPropertyChanged("ScaleX");
		OnPropertyChanged("ScaleY");
		OnPropertyChanged("ScaleZ");
	}

	void	ManagedEntity::raiseTransXChanged()
	{
		OnPropertyChanged("TransX");
	}

	void	ManagedEntity::raiseTransYChanged()
	{
		OnPropertyChanged("TransY");
	}

	void	ManagedEntity::raiseTransZChanged()
	{
		OnPropertyChanged("TransZ");
	}

	void	ManagedEntity::raiseRotXChanged()
	{
		OnPropertyChanged("RotX");
	}

	void	ManagedEntity::raiseRotYChanged()
	{
		OnPropertyChanged("RotY");
	}

	void	ManagedEntity::raiseRotZChanged()
	{
		OnPropertyChanged("RotZ");
	}

	void	ManagedEntity::raiseScaleXChanged()
	{
		OnPropertyChanged("ScaleX");
	}

	void	ManagedEntity::raiseScaleYChanged()
	{
		OnPropertyChanged("ScaleY");
	}

	void	ManagedEntity::raiseScaleZChanged()
	{
		OnPropertyChanged("ScaleZ");
	}

	void	ManagedEntity::raiseAnimationUpdate()
	{
		OnPropertyChanged("CurrentTime");
		OnPropertyChanged("TotalTime");
	}

	void	ManagedEntity::raiseAnimationSpeedChanged()
	{
		OnPropertyChanged("AnimationSpeed");
	}

	void	ManagedEntity::raiseSelectedChanged()
	{
		OnPropertyChanged("Selected");
	}

	void	ManagedEntity::raiseOnScriptRunningChanged()
	{
		OnPropertyChanged("ScriptRunning");
	}

	UnmanagedEntity*
	ManagedEntity::getUnmanaged(void)
	{
		return _unmanaged;
	}

	bool
	ManagedEntity::setMaterialList(void)
	{
		Materials = gcnew System::Collections::Generic::List<ManagedMaterial^>();

		size_t len = _unmanaged->getMaterialsCount();
		for (size_t i = 0 ; i < len ; ++i)
		{
			ManagedMaterial^ mat = gcnew ManagedMaterial();
			mat->setEntityMat(_unmanaged, i);
			Materials->Add(mat);
		}
		return true;
	}
}
