#include "Game/game.hpp"

Game::Game()
{
    Init();
}

void Game::Init()
{
    engine.Init();
    Run();
}

void Game::Run()
{
    while(engine.renderer->windowCloseRequest == false)
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
    glfwPollEvents();

    Transform& cameraTransform = engine.renderer->camera.transform;

    //Player controls
    if(engine.inputMap->GetKey(GLFW_KEY_W))
        cameraTransform.Translate(cameraTransform.Forward());
    
    if(engine.inputMap->GetKey(GLFW_KEY_S))
        cameraTransform.Translate(-cameraTransform.Forward());

    if(engine.inputMap->GetKey(GLFW_KEY_A))
        cameraTransform.Translate(cameraTransform.Right());
    
    if(engine.inputMap->GetKey(GLFW_KEY_D))
        cameraTransform.Translate(-cameraTransform.Right());
}

void Game::Tick()
{
    //Update DeltaTime
    float currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - lastFrame;
    lastFrame = currentFrameTime;
}

void Game::Render()
{
    engine.renderer->Draw();
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}