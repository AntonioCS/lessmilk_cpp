#include "SpriteAnimationSystem.h"
#include "../Component/EnemyComponent.h"
#include <AcsGE/ECS/EntityManager.h>
#include <AcsGE/ECS/Entity.h>
#include <AcsGE/ECS/Components/SpriteAnimationComponent2.h>
#include <AcsGE/Util/Vector2D.h>
#include "../Component/SpriteComponent2.h"


void SpriteAnimationSystem::init()
{
    m_vecRef = getEntityManager()->findByComponent<AcsGameEngine::ECS::SpriteAnimationComponent2, SpriteComponent2>();
}

void SpriteAnimationSystem::update(std::chrono::milliseconds timeStep)
{
    for (const auto &ref : m_vecRef) {
        auto& e = ref.get();
        auto &spriteComp = e.getComponent<SpriteComponent2>();
        auto &spriteAnimationComp = e.getComponent<AcsGameEngine::ECS::SpriteAnimationComponent2>();

        spriteAnimationComp
            .spriteAnimationManager
                .update(timeStep);

        spriteComp.sprite = spriteAnimationComp.
                                spriteAnimationManager
                                    .current();

    }
}

void SpriteAnimationSystem::render()
{
}
