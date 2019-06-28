#include "FontSystem.h"
#include <AcsGE/ECS/EntityManager.h>
#include <AcsGE/Renderer.h>
#include <AcsGE/Texture.h>
#include "../Component/FontComponent.h"
#include "../Component/TransformationComponent.h"
#include "../Component/SpriteComponent2.h"

#include <SDL2/SDL_ttf.h>
#include <iostream>

void FontSystem::init()
{
    m_vecRef = getEntityManager()->findByComponent<FontComponent, TransformationComponent, SpriteComponent2>();
}

void FontSystem::update(std::chrono::milliseconds timeStep)
{
}

void FontSystem::render()
{
    int w, h;

    for (const auto ref : m_vecRef) {
        auto& e = ref.get();

        auto &fontComp = e.getComponent<FontComponent>();

        if (!fontComp.text.empty()) {
            if (m_fontTextures.find(fontComp.text) == m_fontTextures.end()) {
                m_fontTextures[fontComp.text] = getRenderer()->makeTexture(*fontComp.font, fontComp.text, fontComp.color);
            }

            auto &spriteComp = e.getComponent<SpriteComponent2>();
            if (spriteComp.sprite == nullptr) {
                std::cout << "entrei aqui\n";
                TTF_SizeText(fontComp.font->getRawPointer(), fontComp.text.c_str(), &w, &h);
                const SDL_Rect textureSize{ 0, 0, w, h };
                spriteComp.sprite = new AcsGameEngine::Sprite2(&(m_fontTextures[fontComp.text]), textureSize);
            }
        }
    }
}
