#ifndef U_GAME

#include "renderer.hpp"
#include "debugger.hpp"
#include "area.hpp"
#include "input.h"
#include "collision.hpp"

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

    void Start();           //Initial setup
    void Update();          //Update current GameState
    void Draw();            //Draw current GameState

    void QuitGame();        //Close down the application

private:
    bool isRunning = true;

    std::unique_ptr<Renderer> renderer;
    std::unique_ptr<AreaManager> areaManager;
};

#endif
