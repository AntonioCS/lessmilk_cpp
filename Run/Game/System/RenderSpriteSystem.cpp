#include "RenderSpriteSystem.h"

#include <AcsGE/ECS/EntityManager.h>
#include <AcsGE/Renderer.h>
#include <AcsGE/ECS/Components/SpriteAnimationComponent2.h>
#include <AcsGE/ECS/Components/RenderableComponent.h>
#include <AcsGE/ECS/Entity.h>

#include "../Component/SpriteComponent2.h"
#include "../Component/TransformationComponent.h"

#include <SDL2/SDL.h>

using AcsGameEngine::ECS::SpriteAnimationComponent2;
using AcsGameEngine::ECS::RenderableComponent;

void RenderSpriteSystem::init()
{    
    m_vecRef = getEntityManager()->findByComponent<
                                    TransformationComponent, 
                                    SpriteAnimationComponent2, 
                                    SpriteComponent2, 
                                    RenderableComponent
    >();
}

void RenderSpriteSystem::update(std::chrono::milliseconds timeStep)
{
    for (const auto ref : m_vecRef) {
        auto& e = ref.get();
        e.getComponent<RenderableComponent>().hasBeenRendered = false;
    }
}

SDL_RendererFlip flipStateTranslator(TransformationComponent::FlipState &state)
{
    switch (state) {        
        case TransformationComponent::FlipState::Horizontal:
            return SDL_FLIP_HORIZONTAL;
        break;
        case TransformationComponent::FlipState::Vertical:
            return SDL_FLIP_VERTICAL;
        break;
        default:
            return SDL_FLIP_NONE;
    }    
}


void RenderSpriteSystem::render()
{
    for (const auto ref : m_vecRef) {
        auto& e = ref.get();

        auto &spriteComp = e.getComponent<SpriteComponent2>();
        auto &rendComp = e.getComponent<RenderableComponent>();

        if (rendComp.visible && !rendComp.hasBeenRendered && spriteComp.sprite != nullptr) {
            auto &transComp = e.getComponent<TransformationComponent>();
            SDL_Rect destination = {
                transComp.position.getXint(),
                transComp.position.getYint(),
                transComp.size.getXint(),
                transComp.size.getYint()
            };

            SDL_Rect source = spriteComp.sprite->getSource();            
            SDL_Point center = { transComp.center.getXint(), transComp.center.getYint() };
            const SDL_RendererFlip flip = flipStateTranslator(transComp.flipState);

            SDL_RenderCopyEx(
                getRenderer()->getRawPointer(),
                spriteComp.sprite->getTexture()->getRawPointer(),
                &source,
                &destination,
                transComp.rotation,
                (center.x == 0 && center.y == 0 ? nullptr : &center),                
                flip
            );
        }
    }
}
