#pragma once

#include <AcsGE/ECS/Component.h>

namespace AcsGameEngine::ECS
{
    class Entity;
}


class CollidableComponent2 : public AcsGameEngine::ECS::Component
{
public:
    bool hasCollided{false};
    AcsGameEngine::ECS::Entity *e{nullptr};
    bool isDebug{false};
};
