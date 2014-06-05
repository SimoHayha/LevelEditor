#pragma once

#include "../DirectX_Engine/UnmanagedPerf.h"

using namespace System;

class FeedbackPerf;

namespace ManagedLib
{
	public ref class ManagedPerf : public System::ComponentModel::INotifyPropertyChanged
	{
	public:
		delegate void ValueChangedHandler(int i);

	private:
		UnmanagedPerf *			_unmanaged;
		FeedbackPerf *			_feedback;
		ValueChangedHandler^	_valueChanged;

	public:
		ManagedPerf();
		!ManagedPerf();
		~ManagedPerf();

		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;

		void	OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs(info));
		}

		property int Primitives
		{
			int get()
			{
				return _unmanaged->getPrimitives();
			}

			void set(int value)
			{
				_unmanaged->setPrimitives(value);
			}
		};

		property float GpuIdle
		{
			float get()
			{
				return _unmanaged->getGpuIdle();
			}

			void set(float value)
			{
				_unmanaged->setGpuIdle(value);
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

		void			raiseSimpleUpdate(int primitives, float gpuIdle);
		UnmanagedPerf *	getUnmanaged();
	};
}