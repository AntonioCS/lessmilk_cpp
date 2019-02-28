#pragma once

#include <AcsGE/ECS/System.h>


class EnemySystem : public AcsGameEngine::ECS::System
{
public:
    virtual void init() override;
    virtual void update(std::chrono::milliseconds) override;
    virtual void render() override;
};
