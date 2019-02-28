#pragma once

#include <AcsGE/ECS/Component.h>
#include <AcsGE/Sprite.h>


class SpriteComponent2 : public AcsGameEngine::ECS::Component
{
public:
    AcsGameEngine::Sprite *sprite;
};
