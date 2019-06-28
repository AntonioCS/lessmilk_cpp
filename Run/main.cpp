#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <memory>
#include <vector>
#include <utility>
#include <chrono>
#include <array>
#include <random>
#include <unordered_map>

#include <AcsGE/Window.h>
#include <AcsGE/Renderer.h>
#include <AcsGE/AssetManager.h>
#include <AcsGE/EventManager.h>
#include <AcsGE/KeyboardManager.h>
#include <AcsGE/Util/Timer.h>
#include "AcsGE/Util/ColorList.h"
#include <AcsGE/Util/Vector2D.h>
#include <AcsGE/Util/Size.h>

#include <AcsGE/ECS/EntityManager.h>

#include <AcsGE/ECS/Components/SpriteComponent.h>
#include <AcsGE/ECS/Components/RenderableComponent.h>
#include <AcsGE/ECS/Components/PositionComponent.h>
#include <AcsGE/ECS/Components/SpriteAnimationComponent.h>
#include <AcsGE/ECS/Components/SpriteAnimationComponent2.h>
#include <AcsGE/ECS/Components/CollidableComponent.h>

#include <AcsGE/ECS/SystemManager.h>
#include <AcsGE/SpriteAnimation2.h>
#include <AcsGE/Util/Shapes/Shape.h>
#include <AcsGE/Util/Drawer.h>
#include "AcsGE/ECS/Components/DimensionComponent.h"
#include "Game/Component/EnemyComponent.h"
#include "Game/Component/PlayerComponent.h"
#include "Game/Component/CollidableComponent2.h"

#include "Game/System/SpriteAnimationSystem.h"
#include "Game/System/PlayerSystem.h"
#include "Game/System/EnemySystem.h"
#include "Game/System/CollidableSystem2.h"
#include "Game/Component/WorldComponent.h"
#include "Game/Component/TransformationComponent.h"
#include "Game/System/RenderSpriteSystem.h"
#include "Game/Component/WorldPartComponent.h"
#include "Game/Component/SpriteComponent2.h"
#include "Game/System/WorldSystem.h"
#include "Game/Component/FontComponent.h"
#include "Game/Component/ScoreBoardComponent.h"
#include "Game/System/FontSystem.h"
#include "Game/Component/FontMapComponent.h"
#include "Game/System/FontMapSystem.h"
#include "Game/System/ScoreBoardSystem.h"

int main(int argc, char** argv)
{
    using AcsGameEngine::Window;
    using AcsGameEngine::Renderer;

    using AcsGameEngine::EventManager;
    using AcsGameEngine::AssetManager;
    using AcsGameEngine::KeyboardManager;

    using AcsGameEngine::ECS::EntityManager;    

    using AcsGameEngine::ECS::SpriteComponent;
    using AcsGameEngine::ECS::PositionComponent;
    //using AcsGameEngine::ECS::SpriteAnimationComponent;
    using AcsGameEngine::ECS::SpriteAnimationComponent2;
    using AcsGameEngine::ECS::CollidableComponent;
    using AcsGameEngine::ECS::DimensionComponent;
    using AcsGameEngine::ECS::RenderableComponent;

    using AcsGameEngine::Util::Vector2D;
    using AcsGameEngine::Util::Size;


    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Unable to initialize SDL: " << SDL_GetError() << '\n';
        return 1;
    }

    const int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
    if ((IMG_Init(imgFlags) & imgFlags) == 0) {
        std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << '\n';
        return 1;
    }

    if (TTF_Init() == -1) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';

        return 1;
    }

    //NOTE - IMPORTANT
    //The code needs to be in it's own scope so that the SDL shutdown functions don't affect the code
    //I was getting memory issues with the fonts due to the TTF_Quit being called before the cleanup function
    {
        //TTF_Font * font = TTF_OpenFont("Game/assets/fonts/helvica_HELR45W.ttf", 25);
        //AcsGameEngine::Font fo(font);
        //std::cout << "The font max height is: " << TTF_FontHeight(font) << '\n';

        //https://gamedev.stackexchange.com/questions/93225/sdl2-jagged-staircase-edges
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

        constexpr int windowWidth{ 550 };
        constexpr int windowHeight{ 550 };

        const Vector2D worldOffset{ 50, 50 };
        const Size worldSize{ 350, 350 };

        Window window("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight);
        Renderer renderer(window);
        EventManager eventManager;
        AssetManager assetManager(&renderer);
        EntityManager entityManager;
        AcsGameEngine::ECS::SystemManager systemManager(&renderer, &window, &entityManager);
        auto &km = KeyboardManager::getInstance();

        systemManager.addSystem<SpriteAnimationSystem>();
        systemManager.addSystem<PlayerSystem>();
        systemManager.addSystem<EnemySystem>();
        systemManager.addSystem<CollidableSystem2>();
        systemManager.addSystem<WorldSystem>();
        systemManager.addSystem<ScoreBoardSystem>();
        systemManager.addSystem<FontSystem>();
        systemManager.addSystem<FontMapSystem>();
        systemManager.addSystem<RenderSpriteSystem>();

        //Create world entity
        {
            auto& e = entityManager.makeEntity();
            e.addComponent<WorldComponent>();
            auto &transform = e.addComponent<TransformationComponent>();
            transform.position = worldOffset;
            transform.size = worldSize;
        }
 
        {
            ///*
            auto& e = entityManager.makeEntity();
            e.addComponent<ScoreBoardComponent>();
            e.addComponent<SpriteComponent2>();
            auto &transComp = e.addComponent<TransformationComponent>();
            transComp.position = { 10, 10 };
            

            //auto &fontComp = e.addComponent<FontComponent>();
            //fontComp.font = assetManager.loadFont("helvica", "Game/assets/fonts/helvica_HELR45W.ttf", 25);
            //fontComp.text = "score: N";

            assetManager.loadFontMap("arial20", "Game/assets/fonts/Arial/arial_20.png", "Game/assets/fonts/Arial/arial_20.fnt");
            auto fontInfo = assetManager.getFontMap("arial20");

            if (fontInfo) {
                auto &fontMap = e.addComponent<FontMapComponent>();
                fontMap.color = AcsGameEngine::Util::ColorList::red;
                fontMap.text = "score: %d";
                fontMap.fontMapTexture = fontInfo->first;
                fontMap.fontAtlas = fontInfo->second;
                fontMap.size = 20;
            }
            else {
                throw std::logic_error{ "Unable to get map" };
            }

           //*/
        }

        using namespace std::chrono_literals;

        const std::vector<std::pair<std::string, SDL_Rect>> playerMoves
        {
            {
                {"playerMoveDown1", {0, 0, 20, 23}},
                {"playerMoveDown2", {20, 0, 20, 23}},
                {"playerMoveRight1", {0, 25, 20, 23}},
                {"playerMoveRight2", {20, 25, 20, 23}},
                {"playerMoveUp1", {0, 49, 20, 23}},
                {"playerMoveUp2", {20, 49, 20, 23}},
                {"playerMoveLeft1", {0, 73, 20, 23}},
                {"playerMoveLeft2", {20, 73, 20, 23}},
            }
        };

        const Size enemy1WidthHeight{ 32 , 36 };
        const Size enemy2WidthHeight{ 28, 40 };

        const std::vector<std::pair<std::string, SDL_Rect>> enemy1Moves
        {
            {
                {"enemy1Move1", {0, 0, enemy1WidthHeight.getWint(), enemy1WidthHeight.getHint()}},
                {"enemy1Move2", {32, 0, enemy1WidthHeight.getWint(), enemy1WidthHeight.getHint()}}
            }
        };

        const std::vector<std::pair<std::string, SDL_Rect>> enemy2Moves
        {
            {
                {"enemy2Move1", {0, 0, enemy2WidthHeight.getWint(), enemy2WidthHeight.getHint()}},
                {"enemy2Move2", {28, 0, enemy2WidthHeight.getWint(), enemy2WidthHeight.getHint()}}
            }
        };


        //enemy1Moves[0].get(0).["enemy1Move1"]
        assetManager.makeTexture("player", "Game/assets/sprites/player.png");
        assetManager.makeTexture("enemy1", "Game/assets/sprites/enemy1.png");
        assetManager.makeTexture("enemy2", "Game/assets/sprites/enemy2.png");

        assetManager.makeSprites("player", playerMoves);
        assetManager.makeSprites("enemy1", enemy1Moves);
        assetManager.makeSprites("enemy2", enemy2Moves);
        //Player creation
        {
            using AcsGameEngine::SpriteAnimation2;
            auto& playerEntity = entityManager.makeEntity();

            constexpr auto delay = 150ms;
            const Vector2D playerPos{ 40, 40 };
            const Size playerSize{ 20, 23 };

            playerEntity.addComponent<PlayerComponent>();
            playerEntity.addComponent<SpriteComponent2>();
            playerEntity.addComponent<RenderableComponent>();
            playerEntity.addComponent<SpriteAnimationComponent2>()
                .spriteAnimationManager
                .add(
                    "moveDown",
                    SpriteAnimation2{}
                    .addFrame(assetManager.getSprite("playerMoveDown1"), delay)
                    .addFrame(assetManager.getSprite("playerMoveDown2"), delay)
                )
                .add(
                    "moveUp",
                    SpriteAnimation2{}
                    .addFrame(assetManager.getSprite("playerMoveUp1"), delay)
                    .addFrame(assetManager.getSprite("playerMoveUp2"), delay)
                )
                .add(
                    "moveLeft",
                    SpriteAnimation2{}
                    .addFrame(assetManager.getSprite("playerMoveLeft1"), delay)
                    .addFrame(assetManager.getSprite("playerMoveLeft2"), delay)
                )
                .add(
                    "moveRight",
                    SpriteAnimation2{}
                    .addFrame(assetManager.getSprite("playerMoveRight1"), delay)
                    .addFrame(assetManager.getSprite("playerMoveRight2"), delay)
                )
                .setDefaultAnimation("moveLeft");
            playerEntity.addComponent<CollidableComponent2>()
                .isDebug = true;
            auto &transComp = playerEntity.addComponent<TransformationComponent>();
            transComp.position = playerPos;
            transComp.size = playerSize;
        }

        bool is_running{ true };

        if (window.isHidden()) {
            window.showWindow();
        }

        eventManager.onQuit(
            [&is_running](SDL_Event& event)
        {
            is_running = false;
            return true;
        }
        );

        //Enemy creation
        {
            using AcsGameEngine::SpriteAnimation2;
            const auto delay{ 125ms };

            auto enemy1Animation = SpriteAnimation2{}
                .addFrame(assetManager.getSprite("enemy1Move1"), delay)
                .addFrame(assetManager.getSprite("enemy1Move2"), delay);

            auto enemy2Animation = SpriteAnimation2{}
                .addFrame(assetManager.getSprite("enemy2Move1"), delay)
                .addFrame(assetManager.getSprite("enemy2Move2"), delay);

            //std::random_device rd; //Will be used to obtain a seed for the random number engine
            //std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
            //std::uniform_int_distribution<> dis(1, 350);

            //const auto makeEnemy = [&entityManager, &dis, &gen](int quantity, SpriteAnimation2& animation, int w, int h)
            const auto makeEnemy = [&entityManager](int quantity, SpriteAnimation2& animation, const Size &wh)
            {
                const auto x = 0;
                const auto y = 0;

                while (quantity--) {
                    auto& e = entityManager.makeEntity();

                    e.addComponent<EnemyComponent>();
                    e.addComponent<WorldPartComponent>();
                    e.addComponent<SpriteComponent2>();
                    e.addComponent<RenderableComponent>();
                    e.addComponent<SpriteAnimationComponent2>()
                        .spriteAnimationManager
                        .add("moving", animation)
                        .play();
                    //@TODO Refactor this, all the need values are in the TransformationComponent
                    e.addComponent<CollidableComponent2>().isDebug = true;
                    auto &transform = e.addComponent<TransformationComponent>();
                    transform.position = { x, y };
                    transform.size = wh;
                }
            };

            makeEnemy(5, enemy1Animation, enemy1WidthHeight);
            makeEnemy(5, enemy2Animation, enemy2WidthHeight);
        }

        eventManager.onKeyDown([&km](SDL_Event& e)
        {
            km.onKeyDown(e);
            return true;
        });

        eventManager.onKeyUp([&km](SDL_Event& e)
        {
            km.onKeyUp(e);
            return true;
        });

        eventManager.onEvent(SDL_MOUSEBUTTONDOWN, [](SDL_Event &event)
        {
            return true;
        });

        eventManager.onEvent(SDL_MOUSEBUTTONUP, [](SDL_Event &event)
        {
            return true;
        });

        eventManager.onEvent(SDL_MOUSEMOTION, [](SDL_Event &event)
        {
            return true;
        });

        eventManager.onEvent(SDL_MOUSEWHEEL, [](SDL_Event &event)
        {
            return true;
        });


        const AcsGameEngine::Color background{ "#34495E" };

        using clock = std::chrono::high_resolution_clock;
        using std::chrono::milliseconds;

        const auto timeStep{ 16ms };
        AcsGameEngine::Util::Timer::ms accumulator{ 0ms };
        AcsGameEngine::Util::Timer timer;

        timer.start();

        systemManager.init();

        while (is_running) {
            accumulator += timer.elapsed();
            timer.reset();

            eventManager.processEvents();

            while (accumulator >= timeStep) {
                systemManager.update(timeStep);

                accumulator -= timeStep;

                if (accumulator >= 250ms) {
                    accumulator = 0ms;
                }
            }            

            renderer.clear(background);

            systemManager.render();

            renderer.present();
        }
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
