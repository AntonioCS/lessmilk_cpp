#pragma once

#include <AcsGE/ECS/Component.h>
#include <AcsGE/Sprite2.h>


class SpriteComponent2 : public AcsGameEngine::ECS::Component
{
public:
    AcsGameEngine::Sprite2 *sprite;
};
