#include "LuaEntity.h"


LuaEntity::LuaEntity(void)
{
}


LuaEntity::~LuaEntity(void)
{
}

void
LuaEntity::load(Entity* entity)
{
	rotX   = entity->getTransform().rotX;
	rotY   = entity->getTransform().rotY;
	rotZ   = entity->getTransform().rotZ;
	scaleX = entity->getTransform().scaleX;
	scaleY = entity->getTransform().scaleY;
	scaleZ = entity->getTransform().scaleZ;
	transX = entity->getTransform().transX;
	transY = entity->getTransform().transY;
	transZ = entity->getTransform().transZ;
}

void
LuaEntity::store(Entity* entity)
{
	entity->setTransX(transX);
	entity->setTransY(transY);
	entity->setTransZ(transZ);
	entity->setRotateX(rotX);
	entity->setRotateY(rotY);
	entity->setRotateZ(rotZ);
	entity->setScaleX(scaleX);
	entity->setScaleY(scaleY);
	entity->setScaleZ(scaleZ);
}

void
LuaEntity::register_lua(lua_State* L) {
    luabridge::getGlobalNamespace(L)
        .beginNamespace("Entity")
            .beginNamespace("Transform")
                .addVariable("scaleX", &scaleX)
                .addVariable("scaleY", &scaleY)
                .addVariable("scaleZ", &scaleZ)
                .addVariable("rotX", &rotX)
                .addVariable("rotY", &rotY)
                .addVariable("rotZ", &rotZ)
                .addVariable("transX", &transX)
                .addVariable("transY", &transY)
                .addVariable("transZ", &transZ)
            .endNamespace()
        .endNamespace();
}