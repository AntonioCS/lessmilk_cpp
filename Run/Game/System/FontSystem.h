#pragma once

#include <AcsGE/ECS/System.h>

class FontSystem : public AcsGameEngine::ECS::System
{
public:
    void init() override;
    void update(std::chrono::milliseconds) override;
    void render() override;
};
