#pragma once

#include <AcsGE/ECS/Component.h>

class PlayerComponent : public AcsGameEngine::ECS::Component
{
public:
    bool hasCollided{ false };
};
