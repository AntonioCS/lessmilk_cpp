#include "CollidableSystem2.h"

#include <AcsGE/ECS/EntityManager.h>
#include <AcsGE/Renderer.h>
#include <AcsGE/Util/Shapes/Rectangle.h>
#include <AcsGE/Util/CollisionDetection.h>
#include <AcsGE/ECS/Components/PositionComponent.h>
#include <AcsGE/Util/Drawer.h>

#include "../Component/TransformationComponent.h"
#include "../Component/CollidableComponent2.h"

using AcsGameEngine::Util::Shapes::Rectangle;
using AcsGameEngine::Util::CollisionDetection;

void CollidableSystem2::init()
{    
    m_vecRef = getEntityManager()->findByComponent<CollidableComponent2, TransformationComponent>();
}

void CollidableSystem2::update(std::chrono::milliseconds timeStep)
{
    for (const auto ref : m_vecRef) {

        auto &e = ref.get();
        auto &colComp = e.getComponent<CollidableComponent2>();
        auto &transComp = e.getComponent<TransformationComponent>();

        colComp.hasCollided = false;
        colComp.collidadedWith.clear();

        Rectangle currentEntBox{ transComp.position, transComp.size };

        for (const auto ref2 : m_vecRef) {
            if (&e != &(ref2.get())) {
                auto &transCompOther = ref2.get().getComponent<TransformationComponent>();
                Rectangle otherEntBox{ transCompOther.position, transCompOther.size };

                if (CollisionDetection::checkCollision(currentEntBox, otherEntBox)) {
                    colComp.collidadedWith.push_back(&ref2.get());
                }
            }
        }

        colComp.hasCollided = !(colComp.collidadedWith.empty());
    }
} 

void CollidableSystem2::render()
{
    for (const auto ref : m_vecRef) {
        auto &e = ref.get();

        auto &c = e.getComponent<CollidableComponent2>();
        if (c.isDebug) {
            auto &transComp = e.getComponent<TransformationComponent>();
            static AcsGameEngine::Util::Drawer drawer{ *getRenderer() };
            getRenderer()->drawColor(AcsGameEngine::Util::ColorList::red);
            drawer.DrawRect(Rectangle{ transComp.position, transComp.size });
        }
    }
}
