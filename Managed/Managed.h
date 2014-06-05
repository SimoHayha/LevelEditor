#pragma once

#include "..\DirectX_Engine\main.h"
#include "ManagedEntity.h"
#include <string>

using namespace System;

class Feedback;

namespace ManagedLib
{
	public ref class Managed
	{
	public:
		Managed();
		!Managed();
		~Managed();

		IntPtr	init(IntPtr parent, size_t width, size_t height);
		void	run(IntPtr parent);
		IntPtr	addCameraEditor(void);
		IntPtr	addEntity(int type, ManagedEntity^ parent, String^ path);
		IntPtr	addEntity(int type, IntPtr parent, String^ path);
		IntPtr	addEntityToChildModel(int type, IntPtr model, size_t index, String^ path);
		void	removeEntity(ManagedEntity^ entity);
		void	setEntity(ManagedEntity^ entity, int index, ManagedEntity^ parent);
		void	setEntityEditor(ManagedEntity^ entity, int index);
		void	resetScene(void);
		void	raiseClick(void);
		System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>^	getEntities(void);
		void	setCamera(ManagedEntity^ entity, int viewport);
		void	viewportChange(int diff);
		void	resize(int height, int width);
		static void	marshalString(String^ s, std::string& os);
		void	unselect();
		void	actionChanged(String^ action);
		void	wireframe(int cam, bool wireframe);
		void	loadVmf(String^ filename);
	
		delegate void ValueChangedHandler();
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

	private:
		ValueChangedHandler^	_valueChanged;
		CUnmanaged*				m_pUnmanaged;
		Feedback*				_feedback;

		bool	setChildCollection(ManagedEntity^ root);
	};
}
