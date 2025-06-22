#ifndef U_GAME

#include "renderer.hpp"
#include "debugger.hpp"
#include "area.hpp"
#include "input.h"
#include "collision.hpp"
#include "asset_manager.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<AreaManager> areaManager;
    std::unique_ptr<AssetManager> assetManager;
    std::unique_ptr<Debugger> debugger;
    
    std::shared_ptr<InputMap> inputMap;
    std::shared_ptr<Shader> baseShader;

};

#endif
