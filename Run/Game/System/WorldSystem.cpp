﻿#include "WorldSystem.h"
#include "../../AcsGE/ECS/EntityManager.h"
#include "../../AcsGE/Renderer.h"
#include "../Component/WorldComponent.h"
#include "../Component/TransformationComponent.h"
#include "../../AcsGE/Util/Drawer.h"
#include "../../AcsGE/Util/ColorList.h"
#include "../../AcsGE/Util/Shapes/Rectangle.h"


void WorldSystem::init()
{
    //m_vecRef = 
    m_world = &(getEntityManager()->findOneByComponent<WorldComponent>());
}

void WorldSystem::update(std::chrono::milliseconds timeStep)
{
}

void WorldSystem::render()
{
    auto &transComp = m_world->getComponent<TransformationComponent>();
    //getRenderer()->drawRect()

    static AcsGameEngine::Util::Drawer drawer{ *getRenderer() };
    getRenderer()->drawColor(AcsGameEngine::Util::ColorList::red);
    drawer.DrawRect(AcsGameEngine::Util::Rectangle{ transComp.position, transComp.size });
}
