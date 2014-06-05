#pragma once

#include "..\DirectX_Engine\UnmanagedLog.h"

using namespace System;

class FeedbackLog;

namespace ManagedLib
{
	public ref class ManagedLog
	{
	public:
		delegate void ValueChangedHandler(int i);

	private:
		UnmanagedLog*			_unmanaged;
		FeedbackLog*			_feedback;
		ValueChangedHandler^	_valueChanged;
		String^					_str;
	public:
		ManagedLog();
		!ManagedLog();
		~ManagedLog();

		property String^ Value
		{
			String^ get()
			{
				char*	buf = new char[_unmanaged->getLen() + 1];
				_unmanaged->getValue(buf);
				_str = gcnew String(buf);
				delete buf;
				return _str;
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

		void	raiseValueChanged(int i);
	};
}