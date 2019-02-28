#pragma once

#include <AcsGE/ECS/Component.h>
#include <AcsGE/Util/Vector2D.h>
#include <AcsGE/Util/Dimensions.h>


class TransformationComponent : public AcsGameEngine::ECS::Component
{
    using Vector2D = AcsGameEngine::Util::Vector2D;
    //using Dimensions = AcsGameEngine::Util::Dimensions;

public:
    enum class FlipState
    {
        None,
        Horizontal,
        Vertical
    };

    //https://www.youtube.com/watch?v=8_U_pake6J0&list=PLqCJpWy5Fohd6Hg5BW8eAjo5SzQQYl43S&index=7
    Vector2D position; //translation
    Vector2D size;
    Vector2D center;
    //in angles
    double rotation{0};
    FlipState flipState{ FlipState::None };


    //deformation ?
    
};
