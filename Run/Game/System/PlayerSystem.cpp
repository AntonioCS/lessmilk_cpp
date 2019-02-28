#include "PlayerSystem.h"
#include "../Component/PlayerComponent.h"


#include <AcsGE/ECS/EntityManager.h>
#include <AcsGE/ECS/Entity.h>
#include <AcsGE/ECS/Components/SpriteAnimationComponent2.h>
#include <AcsGE/ECS/Components/PositionComponent.h>
#include <AcsGE/KeyboardManager.h>
#include <AcsGE/Util/Vector2D.h>
#include <AcsGE/ECS/Components/SpriteAnimationComponent2.h>
#include "../Component/CollidableComponent2.h"
#include <iostream>
#include "../Component/TransformationComponent.h"


void PlayerSystem::init()
{    
    m_player = &(getEntityManager()->findOneByComponent<PlayerComponent>());
}

void PlayerSystem::update(std::chrono::milliseconds ts)
{
    using AcsGameEngine::ECS::SpriteAnimationComponent2;
    using AcsGameEngine::ECS::PositionComponent;
    using AcsGameEngine::KeyboardManager;
    using AcsGameEngine::Util::Vector2D;


    auto &col = m_player->getComponent<CollidableComponent2>();
    if (col.hasCollided) {
        std::cout << "COLISAO\n";
    }

    const auto &km = KeyboardManager::getInstance();    
    Vector2D speed{ 0.f, 0.f };

    auto& anim = m_player->getComponent<SpriteAnimationComponent2>().spriteAnimationManager;

    if (km.isPressed(KeyboardManager::Keys::ARROW_KEY_UP)) {
        speed.y -= 1.f;
        anim.play("moveUp");
    }
    else if (km.isPressed(KeyboardManager::Keys::ARROW_KEY_DOWN)) {
        speed.y += 1.f;
        anim.play("moveDown");
    }
    else if (km.isPressed(KeyboardManager::Keys::ARROW_KEY_LEFT)) {
        speed.x -= 1.f;
        anim.play("moveLeft");
    }
    else if (km.isPressed(KeyboardManager::Keys::ARROW_KEY_RIGHT)) {
        speed.x += 1.f;
        anim.play("moveRight");
    }
    else {
        anim.stop();
    }

    auto& pos = m_player->getComponent<TransformationComponent>().position;
    constexpr auto speedMultiplier{ 220.f };
    const auto tsValue = (static_cast<float>(ts.count()) / 1000);

    pos.x += speed.x * speedMultiplier * tsValue;
    pos.y += speed.y * speedMultiplier * tsValue;
}

void PlayerSystem::render()
{
}
