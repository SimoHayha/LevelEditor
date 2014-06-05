#include "FBX_Manager.h"
#include "Windows.h"

id::FBX::Manager::Manager(void)
{
}

id::FBX::Manager::~Manager(void)
{
	destroy();
}

void
id::FBX::Manager::destroy(void)
{
	size_t len = scene.size();
	for (size_t i = 0 ; i < len ; ++i)
		scene[i]->Destroy();
	this->manager->Destroy();
	ios->Destroy();
}

bool
id::FBX::Manager::init(void)
{
	this->manager = FbxManager::Create();
	if (this->manager == nullptr)
		return false;
	this->ios = FbxIOSettings::Create(this->manager, IOSROOT);
	if (this->ios == nullptr)
		return false;
	this->manager->SetIOSettings(this->ios);
	return true;
}

bool
id::FBX::Manager::initScene(const char* fileName)
{
	FbxImporter*	importer = FbxImporter::Create(this->manager, "");
	FbxScene*		newScene = FbxScene::Create(this->manager, fileName);

	if (importer == nullptr)
		return (false);
	if (newScene == nullptr || !importer->Initialize(fileName, -1, this->ios))
	{
		importer->Destroy();
		return false;
	}
	importer->Import(newScene);
	newScene->SetName(fileName);
	importer->Destroy();
	try
	{
		this->scene.push_back(newScene);
	}
	catch (std::bad_alloc)
	{
		newScene->Destroy();
		return false;
	}
	return (true);
}

FbxScene*	
id::FBX::Manager::getSceneWithIndex(size_t index) const
{
	return (this->scene[index]);
}

FbxScene*	
id::FBX::Manager::getSceneWithName(const std::string& name)
{
	size_t		size;

	size = this->scene.size();
	for (size_t i = 0; i < size; ++i)
	{
		if (!name.compare(this->scene[i]->GetName()))
			return (this->scene[i]);
	}
	return (nullptr);
}