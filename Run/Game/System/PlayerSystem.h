﻿#pragma once

#include <AcsGE/ECS/System.h>

namespace AcsGameEngine::ECS
{
    class Entity;
}


class PlayerSystem : public AcsGameEngine::ECS::System
{
public:
    virtual void init() override;
    virtual void update(std::chrono::milliseconds) override;
    virtual void render() override;
    void postRender() override;

private:
    bool m_hasCollided{ false };
    AcsGameEngine::ECS::Entity *m_player;
};
