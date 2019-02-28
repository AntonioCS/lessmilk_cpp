#pragma once

#include <AcsGE/ECS/System.h>

class WorldSystem : public AcsGameEngine::ECS::System
{
public:
    void init() override;
    void update(std::chrono::milliseconds) override;
    void render() override;
private:
    AcsGameEngine::ECS::Entity *m_world;
};
