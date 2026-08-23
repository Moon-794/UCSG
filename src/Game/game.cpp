#include "Game/game.hpp"

Game::Game()
{
    Init();
}

void Game::Init()
{
    engine.Init();

    //Setup Asteroids
    for (size_t i = 0; i < 200; i++)
    {
        Asteroid a;
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/200.0f));
        float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/200.0f));
        float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/200.0f));

        float type = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/30.0f));

        if(type > 15.0f)
            a.materialType = "iron";
        else
            a.materialType = "copper";

        a.transform.SetPosition(x, y, z);
        asteroids.push_back(a);
    }
    

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
        cameraTransform.Translate(cameraTransform.Forward() * 0.15f);
    
    if(engine.inputMap->GetKey(GLFW_KEY_S))
        cameraTransform.Translate(-cameraTransform.Forward() * 0.15f);

    if(engine.inputMap->GetKey(GLFW_KEY_A))
        cameraTransform.Translate(cameraTransform.Right() * 0.15f);
    
    if(engine.inputMap->GetKey(GLFW_KEY_D))
        cameraTransform.Translate(-cameraTransform.Right() * 0.15f);
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
    engine.renderer->Clear();

    for (size_t i = 0; i < asteroids.size(); i++)
    {    
        engine.renderer->DrawAsteroid(asteroids[i]);
    }

    engine.renderer->SwapBuffers();
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}