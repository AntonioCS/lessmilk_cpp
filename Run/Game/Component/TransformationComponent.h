#pragma once

#include <AcsGE/ECS/Component.h>
#include <AcsGE/Util/Vector2D.h>
#include <AcsGE/Util/Size.h>


class TransformationComponent : public AcsGameEngine::ECS::Component
{
    using Vector2D = AcsGameEngine::Util::Vector2D;
    using Size = AcsGameEngine::Util::Size;
    //using Dimensions = AcsGameEngine::Util::Dimensions;

public:
    enum class FlipState
    {
        None,
        Horizontal,
        Vertical
    };

    enum class CenterPoint
    {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        MIDDLE_LEFT,
        MIDDLE_CENTER,
        MIDDLE_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT
    };

    //https://www.youtube.com/watch?v=8_U_pake6J0&list=PLqCJpWy5Fohd6Hg5BW8eAjo5SzQQYl43S&index=7
    Vector2D position; //translation
    Size size;
    Vector2D center{0,0};
    //in angles
    double rotation{0};

    FlipState flipState{ FlipState::None };

    int alpha{ 100 };

    Vector2D scale{ 1,1 };

    //deformation ?
    
};
