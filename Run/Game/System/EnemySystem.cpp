#include "EnemySystem.h"
#include "../Component/EnemyComponent.h"

#include <AcsGE/ECS/EntityManager.h>
#include <AcsGE/ECS/Entity.h>
#include <AcsGE/Util/Vector2D.h>
#include <AcsGE/Window.h>
#include <AcsGE/Math/Maths.h>
#include <AcsGE/Util/Random.h>
#include <AcsGE/Renderer.h>
#include <AcsGE/ECS/Components/PositionComponent.h>
#include <AcsGE/Util/ColorList.h>

#include "../Component/TransformationComponent.h"


#include <iostream>

namespace {

    using AcsGameEngine::Window;
    using AcsGameEngine::Util::Vector2D;
    using AcsGameEngine::ECS::PositionComponent;
    using AcsGameEngine::Util::Random;

    void setRandomPositions(const int &winW, const int &winH, const Vector2D &center, Vector2D &start, Vector2D &end)
    {
        ///*
        const float radius{ winW / 2.f + 90 };
        const auto angle = Random::RandomIntBetween(0, 360);

        //https://stackoverflow.com/a/674338/8715
        start.x = center.x + (radius * std::cos(angle));
        start.y = center.y + (radius * std::sin(angle));

        end = start.rotate(center, Random::RandomIntBetween(60, 310));
        //*/

        //start = {100,100};
        //end = {350, 350};
    }

    void setUpEnemy(Window *w, EnemyComponent &eneComp, TransformationComponent &transComp)
    {
        const auto[winW, winH] = w->getSize();
        static const auto winCenter = w->getCenter();

        setRandomPositions(winW, winH, winCenter, eneComp.startPos, eneComp.endPos);

        eneComp.direction = (eneComp.endPos - eneComp.startPos).normalized();
        eneComp.distance = Vector2D::distance(eneComp.startPos, eneComp.endPos);
        transComp.position = eneComp.startPos;
        transComp.rotation = AcsGameEngine::Maths::rotateToFacePointInAngles(eneComp.startPos, eneComp.endPos);

        /*std::cout << "Start pos: " << eneComp.startPos.x << " - " << eneComp.startPos.y << '\n';
        std::cout << "End pos: " << eneComp.endPos.x << " - " << eneComp.endPos.y << '\n';
        std::cout << "Direction: " << eneComp.direction.x << " - " << eneComp.direction.y << '\n';
        std::cout << "Distance: " << eneComp.distance << '\n';*/

    }
}


void EnemySystem::init()
{    
    m_vecRef = getEntityManager()->findByComponent<EnemyComponent>();
    
    for (const auto &ref : m_vecRef) {
        auto& e = ref.get();
        auto &transComp = e.getComponent<TransformationComponent>();

        setUpEnemy(getWindow(), e.getComponent<EnemyComponent>(), transComp);

        transComp.center.x = transComp.size.x / 2;
        transComp.center.y = transComp.size.y / 2;
    }

}

//https://gamedev.stackexchange.com/questions/23447/moving-from-ax-y-to-bx1-y1-with-constant-speed
void EnemySystem::update(std::chrono::milliseconds ts)
{
    using AcsGameEngine::Util::Vector2D;
    using AcsGameEngine::Maths::lerp;
    using AcsGameEngine::ECS::PositionComponent;

    const int speed{ 100 };
    
    for (const auto &ref : m_vecRef) {
        auto& e = ref.get();
        auto &eneComp = e.getComponent<EnemyComponent>();
        auto &transComp = e.getComponent<TransformationComponent>();

        if (eneComp.finished) {
            setUpEnemy(getWindow(), e.getComponent<EnemyComponent>(), e.getComponent<TransformationComponent>());            
            eneComp.finished = false;
        }
        else {

            transComp
                .position += eneComp.direction * speed * msConvert<float>(ts);

            //std::cout << "Current position: " << transComp.position.x << " - " << transComp.position.y << '\n';

            if (Vector2D::distance(eneComp.startPos, transComp.position) >= eneComp.distance) {                
                eneComp.finished = true;
            }
        }
    }
}

void EnemySystem::render()
{    
    for (const auto &ref : m_vecRef) {        
        auto& e = ref.get();
        auto &eneComp = e.getComponent<EnemyComponent>();

        if (eneComp.finished == false) {
            const auto[x1, y1] = eneComp.startPos;
            const auto[x2, y2] = eneComp.endPos;

            getRenderer()->drawColor(AcsGameEngine::Util::ColorList::red);
            getRenderer()->drawLine(x1, y1, x2, y2);

            getRenderer()->drawColor(AcsGameEngine::Util::ColorList::green);
            getRenderer()->drawPoint(x1, y1);

            getRenderer()->drawColor(AcsGameEngine::Util::ColorList::black);
            getRenderer()->drawPoint(x2, y2);
        }
    }
}
