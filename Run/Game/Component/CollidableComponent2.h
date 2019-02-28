#pragma once

#include <AcsGE/ECS/Component.h>
#include <vector>

namespace AcsGameEngine::ECS
{
    class Entity;
}


class CollidableComponent2 : public AcsGameEngine::ECS::Component
{
public:

    enum class ColType
    {
        BOX,
        CIRCLE,
        CUSTOM
    };

    ColType type{ ColType::BOX };    
    bool hasCollided{false};
    std::vector<AcsGameEngine::ECS::Entity*> collidadedWith;
    bool isDebug{false};
};
