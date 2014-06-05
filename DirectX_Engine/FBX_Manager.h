#pragma once

#include <fbxsdk.h>
#include <vector>

namespace id
{
	namespace FBX
	{
		class Manager
		{
		public :
			Manager();
			virtual ~Manager();
			void		destroy();
			bool		init();
			bool		initScene(const char* fileName);
			FbxScene*	getSceneWithIndex(size_t index) const;
			FbxScene*	getSceneWithName(const std::string& name);

		private:
			FbxManager*					manager;
			FbxIOSettings*				ios;
			std::vector<FbxScene*>		scene;
		};
	}
}