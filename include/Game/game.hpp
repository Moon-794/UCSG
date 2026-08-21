#ifndef U_GAME
#define U_GAME

#include "Engine/engine.hpp"

#include "unistd.h"
#include <iostream>
#include <array>
#include <thread>
#include <chrono>

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

    bool isRunning = true;

    Engine engine;

    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<Debugger> debugger;
    std::shared_ptr<Shader> baseShader;

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;
};

#endif
