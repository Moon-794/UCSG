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
    void EndFrame();        //Update GetInputDown, amongst possible other things in the future

    void DrawDebugMenu();

    bool isRunning = true;
    Engine engine;

    //Player related stuff
    float playerWidth = 0.80f;
    float playerHeight = 1.5f;
    Transform playerTransform;
    glm::vec3 playerVelocity;
    PlayerStats stats;

    std::vector<Asteroid> asteroids;
    World world;

    Debugger debugger;
    DebuggerInfo debugInfo;
};

#endif
