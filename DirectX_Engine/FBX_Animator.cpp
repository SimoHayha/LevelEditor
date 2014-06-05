#include "FBX_Animator.h"


id::FBX::Animator::Animator(void) : numAnim(0)
{
	evaluator = nullptr;
	currentAnim = 0;
	loop = false;
	speed = 1.0f;
}


id::FBX::Animator::~Animator(void)
{
}

void	
id::FBX::Animator::setEvaluator(FbxScene* scene)
{
	evaluator = scene->GetEvaluator();
	scene->FillAnimStackNameArray(this->animStackName);
}

void	
id::FBX::Animator::startTime(void)
{
	if (numAnim > 0)
		time = animStack[currentAnim]->LocalStart;
	else
		time.SetSecondDouble(0.0);
}

bool	
id::FBX::Animator::incDt(float dt)
{
	time.SetSecondDouble(time.GetSecondDouble() + (dt * this->speed));
	if (numAnim > 0 && time > animStack[currentAnim]->LocalStop)
		return true;
	return false;
}

void	id::FBX::Animator::setCurrentAnimation(int animation)
{
	this->currentAnim = animation;
	if (this->animStack.size() > 0)
		startStack(currentAnim);
}

FbxAnimEvaluator*
id::FBX::Animator::getEvaluator(void)
{
	return evaluator;
}

FbxTime&
id::FBX::Animator::getTime()
{
	return time;
}

FbxTime
id::FBX::Animator::getTotalTime()
{
	if (currentAnim >= animStack.size())
		return FbxTime(FbxLongLong(0.0f));
	return animStack[currentAnim]->LocalStop;
}

unsigned int&
id::FBX::Animator::getCurrentAnim()
{
	return currentAnim;
}

bool
id::FBX::Animator::addStack(FbxAnimStack* stack)
{
	try
	{
		animStack.push_back(stack);
	}
	catch(std::exception e)
	{
		return false;
	}
	return true;
}

void
id::FBX::Animator::startStack(unsigned int i)
{
	if (numAnim > 0)
	{
		currentAnim = i % animStack.size();
		evaluator->SetContext(animStack[currentAnim]);
	}
	startTime();
}

void
id::FBX::Animator::getGlobalMatrix(FbxAMatrix& matrix, FbxNode* node)
{
	matrix = evaluator->GetNodeGlobalTransform(node, time);
}

void
id::FBX::Animator::getLocalMatrix(FbxAMatrix& matrix, FbxNode* node)
{
	matrix = matrix * evaluator->GetNodeLocalTransform(node, time);
}

bool
id::FBX::Animator::computeDeformations(id::FBX::Mesh& mesh, FbxVector4* newVertices)
{
	int			skinCount;
	int			clusterCount;
	FbxMesh*	myMesh;

	if (!mesh.hasDeformation)
		return true;
	memcpy(newVertices, mesh.node->GetMesh()->GetControlPoints(),
		mesh.node->GetMesh()->GetControlPointsCount() * sizeof(*newVertices));
	if (mesh.hasShape)
		if (!mesh.deformation.ComputeShapeDeformation(newVertices, time))
			return false;
	myMesh = mesh.node->GetMesh();
	skinCount = myMesh->GetDeformerCount(FbxDeformer::eSkin);
	clusterCount = 0;
	for (int i = 0; i < skinCount; ++i)
		clusterCount += ((FbxSkin *)(myMesh->GetDeformer(i, FbxDeformer::eSkin)))->GetClusterCount();
	if (clusterCount)
		if (!mesh.deformation.ComputeSkinDeformation(newVertices, time))
			return false;
	mesh.updateVertexPosition(mesh.node->GetMesh(), newVertices);
	return true;
}

bool
id::FBX::Animator::initAll(FbxScene* scene)
{
	setEvaluator(scene);
	numAnim = scene->GetSrcObjectCount<FbxAnimStack>();
	for (size_t i = 0; i < numAnim ; i++)
    {
        FbxAnimStack* stack = scene->GetSrcObject<FbxAnimStack>(i);
		if (!addStack(stack))
			return false;
    }
	startStack(0);
	return true;
}

size_t	id::FBX::Animator::getNumAnim() const
{
	return this->numAnim;
}

FbxArray<FbxString *> const &	id::FBX::Animator::getAnimNames() const
{
	return this->animStackName;
}

void	id::FBX::Animator::setCurrentAnimationCurrentTime(double d)
{
	this->time.SetSecondDouble(d);
}

bool	id::FBX::Animator::isLoop() const
{
	return this->loop;
}

void	id::FBX::Animator::setLoop(bool b)
{
	this->loop = b;
}

void	id::FBX::Animator::next()
{
	this->currentAnim++;
	if (static_cast<int>(this->currentAnim) > this->animStackName.Size())
		this->currentAnim = 0;
}

void	id::FBX::Animator::setSpeed(float f)
{
	this->speed = f;
}

float	id::FBX::Animator::getSpeed() const
{
	return this->speed;
}