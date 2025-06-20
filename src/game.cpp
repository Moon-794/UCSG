#include "game.hpp"

Game::Game()
{
    Init();
}

void Game::Init()
{
    //Renderer setup
    this->renderer = std::make_unique<Renderer>("Space Game", 2560, 1440);    
    this->renderer->SetClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    debugger = Debugger(renderer.get());

    //Input setup Note: Relies on renderer
    this->inputMap = std::make_shared<InputMap>();
    glfwSetWindowUserPointer(renderer->window, reinterpret_cast<void*>(inputMap.get()));
    glfwSetKeyCallback(renderer->window, key_callback);

    //Resource Manager setup
    resourceManager = std::make_unique<ResourceManager>();

    //AreaManager Setup
    baseShader = resourceManager->GetShader(std::string("base"));   //<----- yucky!
    this->areaManager = std::make_unique<AreaManager>(baseShader); //<----- area manager should not need a shader!!!

    renderer->cameraPos = glm::vec3(16, 16, 0);

    Run();
}

void Game::Run()
{
    while(isRunning)
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        UpdateInputs();
        Tick();
        Render();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        auto timeLeft = std::chrono::microseconds(16670) - frameDuration;

        while (std::chrono::high_resolution_clock::now() < frameEnd + timeLeft) 
        {
            // Just spinning
        }
    }

    QuitGame();
}

void Game::UpdateInputs()
{
    
}

void Game::Tick()
{

}

void Game::Render()
{
    const AreaData& currentArea = areaManager->getCurrentArea();
    renderer->Clear();

    DrawAreaLayer(*renderer, currentArea, 0);
    DebuggerInfo debugInfo = {currentArea, glm::vec2(16, 16)};
    debugger.DrawDebugger(debugInfo);

    renderer->SwapBuffers();

    isRunning = !glfwWindowShouldClose(renderer->window);
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}