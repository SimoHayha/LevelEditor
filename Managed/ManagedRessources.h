#pragma once

#include "..\DirectX_Engine\UnmanagedRessources.h"

using namespace System;
using namespace Collections::Generic;
class FeedbackRessources;

namespace ManagedLib
{
	public ref class ManagedRessources : public System::ComponentModel::INotifyPropertyChanged
	{
	private:
		UnmanagedRessources*	_unmanaged;
		FeedbackRessources*		_feedback;
		List<String^>^			_listFbxModels;
		List<String^>^			_listTextures;
		List<String^>^			_listScripts;
	public:
		ManagedRessources();
		!ManagedRessources();
		~ManagedRessources();

		virtual event System::ComponentModel::PropertyChangedEventHandler^ PropertyChanged;

		void	OnPropertyChanged(String^ info)
		{
			PropertyChanged(this, gcnew System::ComponentModel::PropertyChangedEventArgs(info));
		}

		bool	reloadScript(String^ path);
		bool	addTexture(String^ path);
		bool	loadScript(String^ path);

		property List<String^>^ FbxModels
		{
			List<String^>^ get()
			{
				const std::string* path;
				bool recall = true;
				_listFbxModels = gcnew List<String^>();
				while (recall)
				{
					path = _unmanaged->getNameModels(recall);
					if (path != nullptr)
						_listFbxModels->Add(gcnew String((*path).c_str()));
				}
				return _listFbxModels;
			}

			void set(List<String^>^)
			{
			}
		}

		property List<String^>^ Scripts
		{
			List<String^>^ get()
			{
				const std::string* path;
				bool recall = true;
				_listScripts = gcnew List<String^>();
				while (recall)
				{
					path = _unmanaged->getNameScripts(recall);
					if (path != nullptr)
						_listScripts->Add(gcnew String((*path).c_str()));
				}
				return _listScripts;
			}

			void set(List<String^>^)
			{
			}
		}

		property List<String^>^ Textures
		{
			List<String^>^ get()
			{
				const std::string* path;
				bool recall = true;
				_listTextures = gcnew List<String^>();
				while (recall)
				{
					path = _unmanaged->getNameTextures(recall);
					if (path != nullptr)
						_listTextures->Add(gcnew String((*path).c_str()));
				}
				return _listTextures;
			}

			void set(List<String^>^)
			{
			}
		}

		//void	raiseValueChanged(int i);
	};
}