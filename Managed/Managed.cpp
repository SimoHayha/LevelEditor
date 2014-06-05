#include "stdafx.h"
#include "windows.h"
#include "Managed.h"
#include "Feedback.h"

namespace ManagedLib
{
	Managed::Managed()
	{
	}

	Managed::!Managed()
	{
		delete m_pUnmanaged;
		delete _feedback;
	}

	Managed::~Managed()
	{
	}

	IntPtr Managed::init(IntPtr parent, size_t width, size_t height)
	{
		_feedback = new Feedback(this);
		m_pUnmanaged = new CUnmanaged(_feedback);
		return IntPtr((void*)m_pUnmanaged->initForWPF((HWND)parent.ToPointer(), width, height));
	}

	void Managed::run(IntPtr parent)
	{
		m_pUnmanaged->run((HWND)parent.ToPointer());
	}
	
	void Managed::marshalString(String^ s, std::string& os)
	{
		using namespace Runtime::InteropServices;
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(IntPtr((void*)chars));
	}

	System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>^
	Managed::getEntities(void)
	{
		System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>^ collection;
		try
		{
			collection = gcnew System::Collections::ObjectModel::ObservableCollection<ManagedEntity^>();
			size_t len = m_pUnmanaged->getEntityNb();
			for (size_t i = 0 ; i < len ; ++i)
			{
				collection->Add(gcnew ManagedEntity());
			}
		}
		catch (OutOfMemoryException^)
		{
			return nullptr;
		}
		size_t index = 0;
		for each (ManagedEntity^ it in collection)
		{
			setEntity(it, index, nullptr);
			if (!it->setMaterialList())
				return nullptr;
			if (!setChildCollection(it))
				return nullptr;
			++index;
		}
		return collection;
	}

	void
	Managed::setEntity(ManagedEntity^ entity, int index, ManagedEntity^ parent)
	{
		if (parent == nullptr)
			m_pUnmanaged->setEntity(entity->getUnmanaged(), index);
		else
			m_pUnmanaged->setChildEntity(entity->getUnmanaged(), parent->getUnmanaged(), index);
	}

	void
	Managed::setEntityEditor(ManagedEntity^ entity, int index)
	{
		m_pUnmanaged->setEntityEditor(entity->getUnmanaged(), index);
	}

	bool
	Managed::setChildCollection(ManagedEntity^ root)
	{
		UnmanagedEntity* unmanagedRoot = root->getUnmanaged();
		size_t count = unmanagedRoot->getChildCount();
		for (size_t i = 0 ; i < count ; ++i)
		{
			try
			{
				root->Childs->Add(gcnew ManagedEntity());
			}
			catch (OutOfMemoryException^)
			{
				return false;
			}
			setEntity(root->Childs[i], i, root);
			if (!root->Childs[i]->setMaterialList())
				return false;
			if (!setChildCollection(root->Childs[i]))
				return false;
		}
		return true;
	}

	IntPtr
	Managed::addCameraEditor(void)
	{
		std::string fileName = "";

		return IntPtr(m_pUnmanaged->addEntity(6, nullptr, fileName, true));
	}

	IntPtr
	Managed::addEntity(int type, ManagedEntity^ parent, String^ path)
	{
		UnmanagedEntity* unmanagedParent = nullptr;
		std::string fileName = "";

		if (parent != nullptr)
			unmanagedParent = parent->getUnmanaged();
		if (path != nullptr)
			marshalString(path, fileName);
		return IntPtr(m_pUnmanaged->addEntity(type, unmanagedParent, fileName));
	}

	IntPtr
	Managed::addEntity(int type, IntPtr parent, String^ path)
	{
		std::string fileName = "";

		if (path != nullptr)
			marshalString(path, fileName);
		return IntPtr(m_pUnmanaged->addEntity(type, parent.ToPointer(), fileName));
	}

	IntPtr
	Managed::addEntityToChildModel(int type, IntPtr model, size_t index, String^ path)
	{
		std::string fileName = "";

		if (path != nullptr)
			marshalString(path, fileName);
		return IntPtr(m_pUnmanaged->addEntityToChildModel(type, model.ToPointer(), index, fileName));
	}

	void
	Managed::removeEntity(ManagedEntity^ entity)
	{
		m_pUnmanaged->removeEntity(entity->getUnmanaged());
	}

	void
	Managed::resetScene(void)
	{
		m_pUnmanaged->resetScene();
	}

	void
	Managed::raiseClick(void)
	{
		if (_valueChanged != nullptr)
			_valueChanged->Invoke();
	}

	void
	Managed::setCamera(ManagedEntity^ entity, int viewport)
	{
		if (entity == nullptr || entity->Type != 6)
			return;
		m_pUnmanaged->setCamera(entity->getUnmanaged(), viewport);
	}

	void
	Managed::viewportChange(int diff)
	{
		m_pUnmanaged->viewportChange(diff);
	}

	void
	Managed::resize(int height, int width)
	{
		m_pUnmanaged->resize(height, width);
	}

	void	Managed::unselect()
	{
		m_pUnmanaged->unselect();
	}

	void	Managed::actionChanged(String^ action)
	{
		std::string	str;

		marshalString(action, str);
		m_pUnmanaged->actionChanged(str);
	}

	void	Managed::wireframe(int cam, bool wireframe)
	{
		m_pUnmanaged->wireframe(cam, wireframe);
	}

	void	Managed::loadVmf(String^ filename)
	{
		std::string	str;

		marshalString(filename, str);
		m_pUnmanaged->loadVmf(str);
	}
}