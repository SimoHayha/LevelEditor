#pragma once

#include <cliext\vector>
#include <string>

#include "ManagedMaterial.h"
#include "..\DirectX_Engine\UnmanagedEntity.h"

using namespace System;

class FeedbackEntity;
class SystemClass;

namespace ManagedLib
{
	public ref class ManagedEntity : public System::ComponentModel::INotifyPropertyChanged
	{
	public:
		delegate void ValueChangedHandler(ManagedEntity^ me);

	private:
		UnmanagedEntity*														_unmanaged;
		FeedbackEntity*															_feedback;
		ValueChangedHandler^													_valueChanged;
		System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>^ _childs;
		System::Collections::Generic::List<ManagedMaterial^>^					_materials;
		int																		_viewPortID;

		static void	marshalString(String^ s, std::string& os);

	public:
		ManagedEntity();
		!ManagedEntity();
		~ManagedEntity();

		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;

		void	OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs(info));
		}

		property System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>^ Childs
		{
			System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>^ get()
			{
				return _childs;
			}
			void set(System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>^ value)
			{
				_childs = value;
				OnPropertyChanged("Childs");
			}
		}

		property System::Collections::Generic::List<ManagedMaterial^>^ Materials
		{
			System::Collections::Generic::List<ManagedMaterial^>^ get()
			{
				return _materials;
			}

			void set(System::Collections::Generic::List<ManagedMaterial^>^ v)
			{
				_materials = v;
				OnPropertyChanged("Materials");
			}
		}

		property int ViewPortID
		{
			int get()
			{
				return _unmanaged->getIndexViewport() + 1;
			}
			void set(int nValue)
			{
				if (nValue != _viewPortID)
					_viewPortID = nValue;
			}
		}

		property int Type
		{
			int get()
			{
				return _unmanaged->getValue();
			}
			void set(int nValue)
			{
				_unmanaged->setValue(nValue);
			}
		}

		property String^ Name
		{
			void set(String^ value)
			{
				std::string	str;

				ManagedEntity::marshalString(value, str);

				_unmanaged->setName(str);
				OnPropertyChanged("Name");
			}
			String^ get()
			{
				String^ name = gcnew String(_unmanaged->getName().c_str());
				return name;
			}
		}

		property String^	ScriptName
		{
			String^ get()
			{
				String^ test = gcnew String(_unmanaged->getScriptName().c_str());
				return test;
			}
			void set(String^ v)
			{
				std::string convert;
				using namespace Runtime::InteropServices;
				const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(v)).ToPointer();
				convert = chars;
				Marshal::FreeHGlobal(IntPtr((void*)chars));
				_unmanaged->setScript(convert);
			}
		}

		property float ScaleX
		{
			float get()
			{
				return _unmanaged->getTransform().scaleX;
			}

			void set(float value)
			{
				this->_unmanaged->setScaleX(value);
			}
		}
		property float ScaleY
		{
			float get()
			{
				return _unmanaged->getTransform().scaleY;
			}

			void set(float value)
			{
				this->_unmanaged->setScaleY(value);
			}
		}
		property float ScaleZ
		{
			float get()
			{
				return _unmanaged->getTransform().scaleZ;
			}

			void set(float value)
			{
				this->_unmanaged->setScaleZ(value);
			}
		}

		property float RotX
		{
			float get()
			{
				return _unmanaged->getTransform().rotX;
			}

			void set(float value)
			{
				if (value > 360.0f)
					value = 0.0f;
				else if (value < 0.0f)
					value = 360.f;
				this->_unmanaged->setRotX(value);
			}
		}
		property float RotY
		{
			float get()
			{
				return _unmanaged->getTransform().rotY;
			}

			void set(float value)
			{
				if (value > 360.0f)
					value = 0.0f;
				else if (value < 0.0f)
					value = 360.f;
				this->_unmanaged->setRotY(value);
			}
		}
		property float RotZ
		{
			float get()
			{
				return _unmanaged->getTransform().rotZ;
			}

			void set(float value)
			{
				if (value > 360.0f)
					value = 0.0f;
				else if (value < 0.0f)
					value = 360.f;
				this->_unmanaged->setRotZ(value);
			}
		}

		property float TransX
		{
			float get()
			{
				return _unmanaged->getTransform().transX;
			}

			void set(float value)
			{
				this->_unmanaged->setTransX(value);
			}
		}

		property float TransY
		{
			float get()
			{
				return _unmanaged->getTransform().transY;
			}

			void set(float value)
			{
				this->_unmanaged->setTransY(value);
			}
		}
		property float TransZ
		{
			float get()
			{
				return _unmanaged->getTransform().transZ;
			}

			void set(float value)
			{
				this->_unmanaged->setTransZ(value);
			}
		}

		event ValueChangedHandler^ ValueChanged
		{
			void add(ValueChangedHandler^ pValueChanged)
			{
				_valueChanged = static_cast<ValueChangedHandler^>(Delegate::Combine(_valueChanged, pValueChanged));
			}
			void remove(ValueChangedHandler^ pValueChanged)
			{
				_valueChanged = static_cast<ValueChangedHandler^>(Delegate::Remove(_valueChanged, pValueChanged));
			}
		}

		property System::Collections::Generic::List<String^>^ AnimationNames
		{
			System::Collections::Generic::List<String^>^ get()
			{
				int	size = this->_unmanaged->getAnimNumber();
				System::Collections::Generic::List<String^>^ l = gcnew System::Collections::Generic::List<String^>();

				for (auto i = 0; i < size; ++i)
					l->Add(gcnew String(this->_unmanaged->getAnimNameAt(i)));

				return l;
			}

			void set(System::Collections::Generic::List<String^>^)
			{

			}
		}

		property bool Animate
		{
			bool get()
			{
				return this->_unmanaged->isAnimate();
			}

			void set(bool b)
			{
				this->_unmanaged->setAnimate(b);
			}
		}

		property bool CanAnimate
		{
			bool get()
			{
				return this->_unmanaged->canAnimate();
			}

			void set(bool)
			{
			}
		}

		property int CurrentAnimation
		{
			int get()
			{
				return this->_unmanaged->getCurrentAnimation();
			}

			void set(int animation)
			{
				this->_unmanaged->setCurrentAnimation(animation);
			}
		}

		property double TotalTime
		{
			double get()
			{
				return this->_unmanaged->getCurrentAnimationTotalTime();
			}

			void set(double)
			{
			}
		}

		property double CurrentTime
		{
			double get()
			{
				return this->_unmanaged->getCurrentAnimationCurrentTime();
			}

			void set(double d)
			{
				this->_unmanaged->setCurrentAnimationCurrentTime(d);
			}
		}

		property String^ Path
		{
			String^ get()
			{
				String^	path = gcnew String(_unmanaged->getPath().c_str());
				return path; 
			}
		}

		property bool Loop
		{
			bool get()
			{
				return this->_unmanaged->isAnimationLoop();
			}
			
			void set(bool b)
			{
				this->_unmanaged->setAnimationLoop(b);
			}
		}

		property bool MoveNextOnEnd
		{
			bool get()
			{
				return this->_unmanaged->isAnimationMoveNextOnEnd();
			}

			void set(bool b)
			{
				this->_unmanaged->setAnimationMoveNextOnEnd(b);
			}
		}

		property float AnimationSpeed
		{
			float get()
			{
				return this->_unmanaged->getAnimationSpeed();
			}

			void set(float f)
			{
				this->_unmanaged->setAnimationSpeed(f);
			}
		}

		property bool Selected
		{
			bool get()
			{
				return this->_unmanaged->isSelected();
			}

			void set(bool b)
			{
				this->_unmanaged->setSelected(b);
				OnPropertyChanged("Selected");
			}
		}

		property bool ScriptRunning
		{
			bool get()
			{
				return this->_unmanaged->isScriptRunning();
			}

			void set(bool b)
			{
				this->_unmanaged->setScriptRunning(b);
				OnPropertyChanged("ScriptRunning");
			}
		}
		
		void				raiseValueChanged(int i);
		void				raiseTotalTimeChanged(double d);
		void				raiseCurrentTimeChanged(double d);
		void				raiseCameraMoved();
		void				raiseTransformationUpdate();
		void				raiseTransXChanged();
		void				raiseTransYChanged();
		void				raiseTransZChanged();
		void				raiseRotXChanged();
		void				raiseRotYChanged();
		void				raiseRotZChanged();
		void				raiseScaleXChanged();
		void				raiseScaleYChanged();
		void				raiseScaleZChanged();
		void				raiseAnimationUpdate();
		void				raiseAnimationSpeedChanged();
		void				raiseSelectedChanged();
		void				raiseOnScriptRunningChanged();
		UnmanagedEntity*	getUnmanaged(void);
		bool				setMaterialList(void);
	};
}