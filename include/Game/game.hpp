#ifndef U_GAME
#define U_GAME

#include "Engine/engine.hpp"
#include "Game/game.hpp"
#include "Game/world.hpp"
#include "Game/asteroid.hpp"

#include "unistd.h"
#include <iostream>
#include <array>
#include <thread>
#include <chrono>

struct PlayerStats
{
    int ironAmount = 0;
    int copperAmount = 0;
};

class Game
{
public:
    Game();
    void QuitGame();        //Close down the application

private:

    void Init();            //Initial setup
    void Run();             //Main loop, checks for closed status
    void UpdateInputs();    //Update the input map for this frame
    void Tick();            //Main update function to tick along current GameState
    void Render();          //Draw a scene based on gamestate - The program should work fine with this disabled

    //Gameplay
    void HitAsteroid();
    void CheckAsteroidCollision();
    void CheckShipCollision();

    bool isRunning = true;
    Engine engine;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    //Asteroid Related stuff
    std::vector<Asteroid> asteroids;
    glm::vec3 debugColor = glm::vec3(0.0f, 0.0f, 0.0f);

    //Player related stuff
    float playerWidth = 0.75f;
    float playerHeight = 1.5f;

    Transform playerTransform;
    glm::vec3 playerVelocity;
    PlayerStats stats;
    World world;

    int counter = 0;
};

#endif
