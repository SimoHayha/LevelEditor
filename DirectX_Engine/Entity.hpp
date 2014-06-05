#pragma once

#include "Entity.h"

template <typename T>
T	Entity::getItem(Entity::ItemType item)
{
	switch (item)
	{
	case Entity::ItemType::E_TRANSX:
		return this->getTransX();
	case Entity::ItemType::E_TRANSY:
		return this->getTransY();
	case Entity::ItemType::E_TRANSZ:
		return this->getTransZ();
	case Entity::ItemType::E_ROTATEX:
		return this->getRotX();
	case Entity::ItemType::E_ROTATEY:
		return this->getRotY();
	case Entity::ItemType::E_ROTATEZ:
		return this->getRotZ();
	case Entity::ItemType::E_SCALEX:
		return this->getScaleX();
	case Entity::ItemType::E_SCALEY:
		return this->getScaleY();
	case Entity::ItemType::E_SCALEZ:
		return this->getScaleZ();
	default:
		break;
	}

	return 0;
}

template <typename T>
void	Entity::setItem(Entity::ItemType item, T value)
{
	switch (item)
	{
	case Entity::ItemType::E_TRANSX:
		this->setTransX(value);
		break;
	case Entity::ItemType::E_TRANSY:
		this->setTransY(value);
		break;
	case Entity::ItemType::E_TRANSZ:
		this->setTransZ(value);
		break;
	case Entity::ItemType::E_ROTATEX:
		this->setRotateX(value);
		break;
	case Entity::ItemType::E_ROTATEY:
		this->setRotateY(value);
		break;
	case Entity::ItemType::E_ROTATEZ:
		this->setRotateZ(value);
		break;
	case Entity::ItemType::E_SCALEX:
		this->setScaleX(value);
		break;
	case Entity::ItemType::E_SCALEY:
		this->setScaleY(value);
		break;
	case Entity::ItemType::E_SCALEZ:
		this->setScaleZ(value);
		break;
	default:
		break;
	}
}