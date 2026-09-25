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

    //Handle collisions
    AABB plane1;
    plane1.min = glm::vec3(0.0f, 0.0f, 0.0f);
    plane1.max = glm::vec3(0.0f, 4.0f, 1.0f);

    glm::vec3 pExt = glm::vec3(playerWidth / 2.0f, playerHeight, playerWidth / 2.0f);

    plane1.min = plane1.min - pExt;
    plane1.max = plane1.max + pExt;

    glm::vec3 hitPos;
    glm::vec3 normal;
    if(Physics::Raycast(playerTransform.GetPosition(), playerVelocity, glm::length(playerVelocity), plane1.min, plane1.max, hitPos, normal))
    { 
        playerTransform.SetPosition(hitPos.x, hitPos.y, hitPos.z);
        engine.renderer->DrawDebugCube(hitPos, glm::vec3(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

        float velocityIntoSurface =
        glm::dot(playerVelocity, normal);

        if (velocityIntoSurface < 0.0f)
        {
            playerVelocity -=
                (normal * 1.0025f) * velocityIntoSurface;
        }
    }

    AABB plane2;
    plane2.min = glm::vec3(0.0f, 0.0f, 1.0f);
    plane2.max = glm::vec3(0.0f, 4.0f, 2.0f);

    plane2.min = plane2.min - pExt;
    plane2.max = plane2.max + pExt;

    if(Physics::Raycast(playerTransform.GetPosition(), playerVelocity, glm::length(playerVelocity), plane2.min, plane2.max, hitPos, normal))
    { 
        hitPos += normal * 0.01f;
        playerTransform.SetPosition(hitPos.x, hitPos.y, hitPos.z);
        engine.renderer->DrawDebugCube(hitPos, glm::vec3(0.1f), glm::vec3(0.0f, 1.0f, 0.0f));

        float velocityIntoSurface =
        glm::dot(playerVelocity, normal);

        if (velocityIntoSurface < 0.0f)
        {
            playerVelocity -=
                normal * velocityIntoSurface;
        }
    }

    engine.renderer->DrawLine(playerTransform.GetPosition(), playerTransform.GetPosition() + playerVelocity, glm::vec3(1, 1, 1));

    //Update player based on velocity
    playerTransform.Translate(playerVelocity);
    glm::vec3 camPos = playerTransform.GetPosition() + glm::vec3(0.0f, playerHeight, 0.0f);
    cameraTransform.SetPosition(camPos.x, camPos.y, camPos.z);

    engine.renderer->DrawAABB(plane1, glm::vec3(0, 0, 1));
    engine.renderer->DrawAABB(plane2, glm::vec3(0, 0, 1));
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
