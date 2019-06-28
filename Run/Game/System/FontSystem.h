#pragma once

#include <AcsGE/ECS/System.h>
#include <AcsGE/Texture.h>
#include "AcsGE/Font.h"


namespace AcsGameEngine
{
    class Font;
}

class FontSystem : public AcsGameEngine::ECS::System
{
public:
    void init() override;
    void update(std::chrono::milliseconds) override;
    void render() override;

private:
    using Texture = AcsGameEngine::Texture;
    //Texture *makeTexture(AcsGameEngine::Font *font, std::string text, int size);


    std::unordered_map<std::string, Texture> m_fontTextures;
};
