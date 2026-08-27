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

        a.transform.SetPosition(x - 100.0f, y - 100.0f, z - 100.0f);
        asteroids.push_back(a);
    }

    world.Init();

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

    float movespeed = 0.15f;

    if(engine.inputMap->GetKey(GLFW_KEY_LEFT_SHIFT))
        movespeed = 0.0002f;

    //Player controls
    if(engine.inputMap->GetKey(GLFW_KEY_W))
        cameraTransform.Translate(cameraTransform.Forward() * movespeed);
    
    if(engine.inputMap->GetKey(GLFW_KEY_S))
        cameraTransform.Translate(-cameraTransform.Forward() * movespeed);

    if(engine.inputMap->GetKey(GLFW_KEY_A))
        cameraTransform.Translate(cameraTransform.Right() * movespeed);
    
    if(engine.inputMap->GetKey(GLFW_KEY_D))
        cameraTransform.Translate(-cameraTransform.Right() * movespeed);
    
    if(engine.inputMap->GetKeyDown(GLFW_KEY_SPACE))
       HitAsteroid();

    engine.inputMap->SetKeyDown();
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

void Game::HitAsteroid()
{
    for (size_t i = 0; i < asteroids.size(); i++)
    {
        glm::vec3 hitPosition = glm::vec3(0.0f, 0.0f, 0.0f);
        if(Physics::Raycast(engine.renderer->camera.transform.GetPosition(), engine.renderer->camera.transform.Forward(), asteroids[i].transform.GetPosition(), hitPosition))
        {
            asteroids[i].remainingMaterial -= 20;
            if(asteroids[i].remainingMaterial <= 0)
            {
                asteroids[i].isActive = false;
            }
            return;
        }
    }
    
    std::cout << "No hit detected..." << std::endl;
    debugColor = glm::vec3(1, 0, 0);
    return;
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}
