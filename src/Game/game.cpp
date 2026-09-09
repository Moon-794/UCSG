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

    asteroids[0].transform.SetPosition(32, 0, 32);
    asteroids[0].transform.SetScale(0.1f, 0.1f, 0.1f);

    world.Init();
    engine.renderer->UpdateShipMesh(world);

    engine.renderer->camera.transform.SetPosition(3, 2, 3);

    playerTransform.SetPosition(3, 0, 3);

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
    glm::vec3 camForward = glm::vec3(cameraTransform.Forward().x, 0.0f, cameraTransform.Forward().z);

    float movespeed = 0.075f;
    playerVelocity = glm::vec3(0.0f, 0.0f, 0.0f);

    if(engine.inputMap->GetKey(GLFW_KEY_LEFT_SHIFT))
        movespeed = 0.025f;

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

    if(engine.inputMap->GetKeyDown(GLFW_KEY_SPACE))
       HitAsteroid();

    //Update player based on velocity
    playerTransform.Translate(playerVelocity);
    glm::vec3 camPos = playerTransform.GetPosition() + glm::vec3(0.0f, playerHeight, 0.0f);
    cameraTransform.SetPosition(camPos.x, camPos.y, camPos.z);

    engine.inputMap->SetKeyDown();
}

void Game::Tick()
{
    //Update DeltaTime
    float currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - lastFrame;
    lastFrame = currentFrameTime;

    CheckShipCollision();
    CheckAsteroidCollision();
}

void Game::CheckAsteroidCollision()
{
    glm::vec3 playerOffset =  glm::vec3(playerWidth / 2, 0.0f, playerWidth / 2);
    glm::vec3 minPlayer = playerTransform.GetPosition() - playerOffset;
    glm::vec3 maxPlayer = playerTransform.GetPosition() + playerOffset + glm::vec3(0.0f, playerHeight, 0.0f);
    glm::vec3 boxCenter = glm::vec3(0.5f, 0.5f, 0.5f);

    for (size_t i = 0; i < asteroids.size(); i++)
    {
        glm::vec3 boxMin = asteroids[i].transform.GetPosition() - boxCenter;
        glm::vec3 boxMax = asteroids[i].transform.GetPosition() + boxCenter; 

        if(Physics::AABB_AABB_Collision(minPlayer, maxPlayer, boxMin, boxMax))
        {
            Physics::Resolve_AABB_AABB_Collision(minPlayer, maxPlayer, boxMin, boxMax, playerTransform);

            glm::vec3 camPos = playerTransform.GetPosition() + glm::vec3(0.0f, playerHeight, 0.0f);
            engine.renderer->camera.transform.SetPosition(camPos.x, camPos.y, camPos.z);
        }
    }
}

void Game::CheckShipCollision()
{
    glm::vec3 pos = glm::floor(playerTransform.GetPosition());
    glm::vec3 playerOffset =  glm::vec3(playerWidth / 2, 0.0f, playerWidth / 2);
    glm::vec3 minPlayer = playerTransform.GetPosition() - playerOffset;
    glm::vec3 maxPlayer = playerTransform.GetPosition() + playerOffset + glm::vec3(0.0f, playerHeight, 0.0f);

    glm::vec3 planeMin;
    glm::vec3 planeMax;

    //3x3 ship grid around player
    for (int i = -1; i < 2; i++)
    {
        for (int k = -1; k < 2; k++)
        {
            if(pos.x + i >= 0 && pos.x + i < 32 && pos.z + k >= 0 && pos.z + k < 32)
            {
                int x = pos.x + i;
                int z = pos.z + k;

                if(world.shipGrid[x][z] == TileType::empty)
                    continue;
                
                //Left wall
                if(x == 0 || world.shipGrid[x - 1][z] == TileType::empty)
                {
                    
                }
            }
        }
               
    }

    glm::vec3 camPos = playerTransform.GetPosition() + glm::vec3(0.0f, playerHeight, 0.0f);
    engine.renderer->camera.transform.SetPosition(camPos.x, camPos.y, camPos.z);
}

void Game::Render()
{
    engine.renderer->Clear();

    for (size_t i = 0; i < asteroids.size(); i++)
    {    
        engine.renderer->DrawAsteroid(asteroids[i]);
    }

    engine.renderer->DrawShip();

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
