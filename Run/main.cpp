
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

#include "AcsGE/Window.h"
#include "AcsGE/Renderer.h"
#include "AcsGE/AssetManager.h"
#include "AcsGE/EventManager.h"
#include "AcsGE/Util/Timer.h"
#include "AcsGE/Util/ColorList.h"
#include "AcsGE/Util/Vector2D.h"

#include "AcsGE/ECS/EntityManager.h"
#include "AcsGE/ECS/Components/SpriteComponent.h"
#include "AcsGE/ECS/Components/RenderableComponent.h"
#include "AcsGE/ECS/Components/PositionComponent.h"
#include "AcsGE/ECS/SystemManager.h"



float operator*(float a, std::chrono::milliseconds b)
{    
    return a * (static_cast<float>(b.count()) / 1000);
}

struct Directions
{
    bool left{ false };
    bool right{ false };
    bool up{ false };
    bool down{ false };
} directions;


struct Entity
{
    AcsGameEngine::Util::Vector2D vec;
    AcsGameEngine::Sprite *sprite;
};

struct EntityEnemy : Entity
{
    bool moving{ false };
    int angle;

    Directions directions;
};

int main(int argc, char **argv)
{
    using AcsGameEngine::Window;
    using AcsGameEngine::Renderer;
    using AcsGameEngine::EventManager;
    using AcsGameEngine::AssetManager;

    using AcsGameEngine::ECS::EntityManager;
    using AcsGameEngine::ECS::SpriteComponent;

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

    Window window("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 350, 350);
    Renderer renderer(window);
    EventManager eventManager;
    AssetManager assetManager(&renderer);
    EntityManager entityManager;
    
    const std::vector<std::pair<std::string, SDL_Rect>> playerMoves
    {
        {
            {"playerMoveDown1", {0,0,20,23}},
            {"playerMoveDown2", {20,0,20,23}},
            {"playerMoveRight1", {0,25,20,23}},
            {"playerMoveRight2", {20,25,20,23}},
            {"playerMoveUp1", {0,49,20,23}},
            {"playerMoveUp2", {20,49,20,23}},
            {"playerMoveLeft1", {0,73,20,23}},
            {"playerMoveLeft2", {20,73,20,23}},
        }
    };
    const std::vector<std::pair<std::string, SDL_Rect>> enemy1Moves
    {
        {
            {"enemy1Move1", {0,0,32,36}},
            {"enemy1Move2", {0,32,32,36}}
        }
    };

    const std::vector<std::pair<std::string, SDL_Rect>> enemy2Moves
    {
        {
            {"enemy2Move1", {0,0,28,40}},
            {"enemy2Move2", {28,0,28,40}}            
        }
    };


    assetManager.makeTexture("player", "assets/player.png");
    assetManager.makeTexture("enemy1", "assets/enemy1.png");
    assetManager.makeTexture("enemy2", "assets/enemy2.png");

    assetManager.makeSprites("player", playerMoves);
    assetManager.makeSprites("enemy1", enemy1Moves);
    assetManager.makeSprites("enemy2", enemy2Moves);

    auto playerEntity = entityManager.makeEntity();
    //playerEntity.addComponent<SpriteComponent>(assetManager.getSprite("playerMoveRight1"));
    //playerEntity.addComponent<SpriteComponent>(assetManager.getSprite("playerMoveRight1"));




    bool is_running{true};

    using namespace std::chrono_literals;
    using clock = std::chrono::high_resolution_clock;
    using std::chrono::milliseconds;

    AcsGameEngine::Util::Timer timer;
    timer.start();

    constexpr milliseconds timeStep{ 16ms };
    auto accumulator{ 0ms };

    if (window.isHidden())
    {
        window.showWindow();
    }

    eventManager.onQuit(
        [&is_running](SDL_Event &event) {
            is_running = false;
        }
    );
    
    const AcsGameEngine::Color background{"#34495E"};

    Entity player;

    player.sprite = assetManager.getSprite("playerMoveRight1");
    player.vec.x = 40;
    player.vec.y = 40;

    std::array<EntityEnemy, 6> enemiesArray;

    enemiesArray[0].sprite = assetManager.getSprite("enemy1Move1");
    enemiesArray[1].sprite = assetManager.getSprite("enemy1Move1");
    enemiesArray[2].sprite = assetManager.getSprite("enemy1Move1");

    enemiesArray[3].sprite = assetManager.getSprite("enemy2Move1");
    enemiesArray[4].sprite = assetManager.getSprite("enemy2Move1");
    enemiesArray[5].sprite = assetManager.getSprite("enemy2Move1");

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> dis(1, 350);

    for (auto &e : enemiesArray)
    {
        e.vec.x = dis(gen);
        e.vec.y = dis(gen);        
    }


    eventManager.onKeyUp(
        [](SDL_Event &e) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    directions.up = false;
                    break;
                case SDLK_DOWN:
                    directions.down = false;
                    break;
                case SDLK_LEFT:
                    directions.left = false;
                    break;
                case SDLK_RIGHT:
                    directions.right = false;
                    break;
                default:
                    break;
            }
        }
    );

    eventManager.onKeyDown(
        [](SDL_Event &e) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    directions.up = true;
                    break;
                case SDLK_DOWN:
                    directions.down = true;
                    break;
                case SDLK_LEFT:
                    directions.left = true;
                    break;
                case SDLK_RIGHT:
                    directions.right = true;
                    break;
                default:
                    break;
            }            
        }
    );


    while (is_running)
    {
        accumulator += timer.elapsed();
        timer.reset();

        do
        {
            eventManager.processEvents();
            AcsGameEngine::Util::Vector2D speed{ 0.f, 0.f };

            //std::cout << "Directions " << directions.up << " - " << directions.down << " --- "
              //  << directions.left << " --- " << directions.right << '\n';

            if (directions.up) speed.y -= 1.f;
            if (directions.down) speed.y += 1.f;
            if (directions.left) speed.x -= 1.f;
            if (directions.right) speed.x += 1.f;

            for (auto &e : enemiesArray)
            {
                if (!e.moving)
                {
                    e.moving = true;
                    //e.vec.
                    //e.sprite->setDestinationXY(e.vec.x, e.vec.y);
                }
            }


            std::cout << "Speed factor: " << static_cast<float>(20.f * timeStep) << '\n';
            player.vec.x += speed.x *20.f * timeStep;
            player.vec.y += speed.y *20.f * timeStep;

            player.sprite->setDestinationXY(player.vec.x, player.vec.y);



            //currentState.update(timeStep);

            accumulator -= timeStep;
        } while (accumulator >= timeStep);

        renderer.clear(background);

        renderer.drawSprite(player.sprite);
        for (auto &e : enemiesArray) {
            e.sprite->setDestinationXY(e.vec.x, e.vec.y);
            renderer.drawSprite(e.sprite);
        }
        renderer.present();
    }

    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}
