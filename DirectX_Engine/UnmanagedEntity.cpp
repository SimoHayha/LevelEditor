#include "UnmanagedEntity.h"
#include "Model.h"
#include "Log.hpp"
#include "Entity.h"
#include "MovementController.h"

UnmanagedEntity::UnmanagedEntity(FeedbackEntity* feedback)
{
	_feedback = feedback;
	_value = 0;
}

void
UnmanagedEntity::setValue(int pValue)
{
	_value = pValue;
	_feedback->OnValueChanged(pValue);
}

int
UnmanagedEntity::getValue(void)
{
	return _entity->getType();
}


size_t
UnmanagedEntity::getIndex(void)
{
	return _index;
}

void
UnmanagedEntity::setEntity(size_t index, Entity* entity)
{
	_index = index;
	_entity = entity;
}

UnmanagedEntity::Transform&
UnmanagedEntity::getTransform(void)
{
	return _entity->getTransform();
}

void	UnmanagedEntity::setTransX(float f)
{
	this->_entity->setTransX(f);
}

void	UnmanagedEntity::setTransY(float f)
{
	this->_entity->setTransY(f);
}

void	UnmanagedEntity::setTransZ(float f)
{
	this->_entity->setTransZ(f);
}

void	UnmanagedEntity::setRotX(float f)
{
	this->_entity->setRotateX(f);
}

void	UnmanagedEntity::setRotY(float f)
{
	this->_entity->setRotateY(f);
}

void	UnmanagedEntity::setRotZ(float f)
{
	this->_entity->setRotateZ(f);
}

void	UnmanagedEntity::setScaleX(float f)
{
	this->_entity->setScaleX(f);
}

void	UnmanagedEntity::setScaleY(float f)
{
	this->_entity->setScaleY(f);
}

void	UnmanagedEntity::setScaleZ(float f)
{
	this->_entity->setScaleZ(f);
}

size_t	UnmanagedEntity::getChildCount(void)
{
	return _entity->getChildCount();
}

Entity*	UnmanagedEntity::getEntity(void)
{
	return _entity;
}

int		UnmanagedEntity::getAnimNumber()
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return 0;

		FbxArray<FbxString *> const &	array = m->getAnimator()->getAnimNames();

		return array.Size();
	}

	return 0;
}

char *	UnmanagedEntity::getAnimNameAt(int index)
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return 0;

		FbxArray<FbxString *> const &	array = m->getAnimator()->getAnimNames();

		if (index >= array.Size())
			return "Undefined";

		return array[index]->Buffer();
	}

	return "Undefined";
}

void	UnmanagedEntity::setAnimate(bool b)
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return ;

		m->setAnimate(b);
	}
}

bool	UnmanagedEntity::isAnimate()
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return false;

		return m->isAnimate();
	}

	return false;
}

bool	UnmanagedEntity::canAnimate()
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return false;

		return (m->getAnimator()->getNumAnim() > 0);
	}

	return false;
}

void	UnmanagedEntity::setCurrentAnimation(int animation)
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return ;

		if (animation < 0 || animation > (int)m->getAnimator()->getNumAnim())
			animation = 0;

		m->setCurrentAnimation(animation);
	}
}

int	UnmanagedEntity::getCurrentAnimation()
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return 0;

		return m->getCurrentAnimation();
	}

	return 0;
}

double	UnmanagedEntity::getCurrentAnimationTotalTime()
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return 0.0;

		double tmp = m->getCurrentAnimationTotalTime();

		return m->getCurrentAnimationTotalTime();
	}

	return 0.0;
}

double	UnmanagedEntity::getCurrentAnimationCurrentTime()
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return 0.0;

		return m->getCurrentAnimationCurrentTime();
	}

	return 0.0;
}

void	UnmanagedEntity::setCurrentAnimationCurrentTime(double d)
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return ;

		m->setCurrentAnimationCurrentTime(d);
	}
}

std::string&	UnmanagedEntity::getPath(void)
{
	return _entity->getFilePath();
}

size_t
UnmanagedEntity::getMaterialsCount(void)
{
	return _entity->getSubMeshCount();
}

std::string&
UnmanagedEntity::getMaterialName(size_t index)
{
	return _entity->getSubMeshName(index);
}

std::string&
UnmanagedEntity::getName(void)
{
	return _entity->getName();
}

int	
UnmanagedEntity::getIndexViewport(void)
{
	MovementController*	m = dynamic_cast<MovementController*>(this->_entity);
		if (nullptr == m)
			return -1;
	return m->viewport;
}

std::string&
UnmanagedEntity::getTextureName(size_t index)
{
	return _entity->getTextureName(index);
}

void
UnmanagedEntity::setTexture(size_t index, const std::string& name)
{
	_entity->setTexture(index, name);
}

std::string&
UnmanagedEntity::getScriptName(void)
{
	return _entity->getScriptName();
}

void
UnmanagedEntity::setScript(const std::string& name)
{
	_entity->setScript(name);
}

float	UnmanagedEntity::getTransX() const
{
	return _entity->getTransform().transX;
}

float	UnmanagedEntity::getTransY() const
{
	return _entity->getTransform().transY;
}

float	UnmanagedEntity::getTransZ() const
{
	return _entity->getTransform().transZ;
}

float	UnmanagedEntity::getRotX() const
{
	return _entity->getTransform().rotX;
}

float	UnmanagedEntity::getRotY() const
{
	return _entity->getTransform().rotY;
}

float	UnmanagedEntity::getRotZ() const
{
	return _entity->getTransform().rotZ;
}

float	UnmanagedEntity::getScaleX() const
{
	return _entity->getTransform().scaleX;
}

float	UnmanagedEntity::getScaleY() const
{
	return _entity->getTransform().scaleY;
}

float	UnmanagedEntity::getScaleZ() const
{
	return _entity->getTransform().scaleZ;
}

void	UnmanagedEntity::setName(std::string & s)
{
	this->_entity->setName(s);
}

bool	UnmanagedEntity::isAnimationLoop() const
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return false;

		return m->isAnimationLoop();
	}

	return false;
}

void	UnmanagedEntity::setAnimationLoop(bool b)
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return ;

		m->setAnimationLoop(b);
	}
}

bool	UnmanagedEntity::isAnimationMoveNextOnEnd() const
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return false;

		return m->isAnimationMoveNextOnEnd();
	}

	return false;
}

void	UnmanagedEntity::setAnimationMoveNextOnEnd(bool b)
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return ;

		m->setAnimationMoveNextOnEnd(b);
	}
}

float	UnmanagedEntity::getAnimationSpeed() const
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return 0.0f;

		return m->getAnimationSpeed();
	}

	return 0.0f;
}

void	UnmanagedEntity::setAnimationSpeed(float f)
{
	if (Entity::E_MODEL == this->_entity->getType())
	{
		Model *	m = dynamic_cast<Model *>(this->_entity);
		if (nullptr == m)
			return ;

		m->setAnimationSpeed(f);
	}
}

bool	UnmanagedEntity::isSelected() const
{
	return this->_entity->isSelected();
}

void	UnmanagedEntity::setSelected(bool b)
{
	this->_entity->setSelected(b);
}

bool	UnmanagedEntity::isScriptRunning() const
{
	return this->_entity->isScriptRunning();
}

void	UnmanagedEntity::setScriptRunning(bool b)
{
	this->_entity->setScriptRunning(b);
}