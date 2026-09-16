#include "Game/game.hpp"

Game::Game()
{
    Init();
}

void Game::Init()
{
    engine.Init();

    //Setup Asteroids
    for (size_t i = 0; i < 20; i++)
    {
        Asteroid a;
        float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/50.0f));
        float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/1.0f));
        float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/50.0f));

        float type = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/30.0f));

        if(type > 15.0f)
            a.materialType = "iron";
        else
            a.materialType = "copper";

        a.transform.SetPosition(x - 25, y, z - 25);
        asteroids.push_back(a);
    }

    world.Init();
    engine.renderer->UpdateShipMesh(world);

    engine.renderer->camera.transform.SetPosition(3, 2, 3);
    playerTransform.SetPosition(3, 0, 3);

    debugger.InitImGUI(engine.renderer->window);

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
}

void Game::Tick()
{   
    engine.renderer->DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0.25f, 0.75f, 0.25f));
    engine.renderer->DrawLine(glm::vec3(0, 0, 0), glm::vec3(0, 4, 0), glm::vec3(0.25f, 0.75f, 0.25f));
    engine.renderer->DrawLine(glm::vec3(0, 4, 0), glm::vec3(0, 4, 1), glm::vec3(0.25f, 0.75f, 0.25f));
    engine.renderer->DrawLine(glm::vec3(0, 4, 1), glm::vec3(0, 0, 1), glm::vec3(0.25f, 0.75f, 0.25f));

    Transform& cameraTransform = engine.renderer->camera.transform;
    glm::vec3 camForward = glm::vec3(cameraTransform.Forward().x, 0.0f, cameraTransform.Forward().z);

    float movespeed = 0.075f;
    playerVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

    //Player controls
    if(engine.inputMap->GetKey(GLFW_KEY_W))
        playerVelocity += camForward;
    
    if(engine.inputMap->GetKey(GLFW_KEY_S))
        playerVelocity -= camForward;

    if(engine.inputMap->GetKey(GLFW_KEY_A))
        playerVelocity += cameraTransform.Right();
    
    if(engine.inputMap->GetKey(GLFW_KEY_D))
        playerVelocity -= cameraTransform.Right();
    
    if(glm::length(playerVelocity) > movespeed && glm::length(playerVelocity) != 0)
    {
        playerVelocity = glm::normalize(playerVelocity) * movespeed;
    }

    //Update player based on velocity
    playerTransform.Translate(playerVelocity);
    glm::vec3 camPos = playerTransform.GetPosition() + glm::vec3(0.0f, playerHeight, 0.0f);
    cameraTransform.SetPosition(camPos.x, camPos.y, camPos.z);
}

void Game::Render()
{
    engine.renderer->Clear();

    for (size_t i = 0; i < asteroids.size(); i++)
    {   
        engine.renderer->DrawAsteroid(asteroids[i]);
    }

    engine.renderer->DrawShip();

    DrawDebugMenu();

    engine.renderer->FlushLines();

    engine.renderer->SwapBuffers();
}

void Game::EndFrame()
{
    engine.inputMap->SetKeyDown();
}

void Game::DrawDebugMenu()
{
    debugInfo.playerPosition = playerTransform.GetPosition();
    debugger.Draw(*engine.renderer, debugInfo);
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}
