#pragma once
#include <AcsGE/ECS/Component.h>
#include <AcsGE/Util/Vector2D.h>


class EnemyComponent : public AcsGameEngine::ECS::Component
{
    using Vector2D = AcsGameEngine::Util::Vector2D;
public:
    bool finished{ false };
    bool started{ false };

    Vector2D startPos;
    Vector2D endPos;
    Vector2D direction;

    float distance;
};
