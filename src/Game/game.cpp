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

    HandleWallCollisions();
    
    //Update player based on velocity
    playerTransform.Translate(playerVelocity);
    glm::vec3 camPos = playerTransform.GetPosition() + glm::vec3(0.0f, playerHeight, 0.0f);
    cameraTransform.SetPosition(camPos.x, camPos.y, camPos.z);

    //engine.renderer->DrawAABB(plane1, glm::vec3(0, 0, 1));
    //engine.renderer->DrawAABB(plane2, glm::vec3(0, 0, 1));
}

void Game::HandleWallCollisions()
{
    std::vector<AABB> candidates;
    std::vector<Physics::CollisionData> collisions;
    glm::vec3 rounded = glm::floor(playerTransform.GetPosition());
    glm::vec3 pPos = playerTransform.GetPosition();
    glm::vec3 pExt = glm::vec3(playerWidth / 2.0f, playerHeight, playerWidth / 2.0f);

    //Get all the data
    for (int i = -1; i < 2; i++)
    {
        for (int j = -1; j < 2; j++)
        {
            if(rounded.x + i >= 0 && rounded.x + i < 32)
            {
                if(rounded.z + j >= 0 && rounded.z + j < 32)
                {
                    if(world.shipGrid[rounded.x + i][rounded.z + j] == TileType::ship)
                    {
                        int x = rounded.x + i;
                        int z = rounded.z + j;
                        glm::vec3 tilePos = glm::vec3(rounded.x + i, 0.0f, rounded.z + j);

                        //Left Wall check
                        if(rounded.x + i - 1 < 0 || world.shipGrid[rounded.x + i - 1][rounded.z + j] == TileType::empty)
                        {
                            AABB planeLeft;
                            planeLeft.min = glm::vec3(x, 0.0f, z) - pExt;
                            planeLeft.max = glm::vec3(x, 4.0f, z + 1) + pExt;
                            candidates.push_back(planeLeft);
                        }
                    }
                }
            }
        }
    }

    if(collisions.size() != 0)
    {
        std::sort(collisions.begin(), collisions.end(), [](const Physics::CollisionData &a, const Physics::CollisionData &b)
        {
            return a.time < b.time;
        });
    } 

    //ALL candidates retrieved
    //C = MAX_COLLISIONS
    float shortest = 100;
    for (size_t C = 0; C < MAX_COLLISIONS; C++)
    {
        Physics::CollisionData cData;
        cData.normal = glm::vec3(0, 0, 0);
        cData.time = 0;
        cData.hitPos = glm::vec3(0, 0, 0);

        if(candidates.size() == 0)
        {
            break;
        }

        for (size_t i = 0; i < candidates.size(); i++)
        {
            glm::vec3 hit;
            glm::vec3 normal;
            float time;

            AABB candidate = candidates[i];
            if(Physics::Raycast(pPos, playerVelocity, glm::length(playerVelocity), candidate.min, candidate.max, hit, normal, time))
            {
                if(time < shortest)
                {
                    shortest = time;
                    cData.time = time;
                    cData.hitPos = hit;
                    cData.normal = normal;
                }
            }
        }

        if(cData.normal == glm::vec3(0, 0, 0))
        {
            break;
        }
    
        //Ok we have our shortest hit, resolve
        cData.hitPos += cData.normal * (0.01f);
        playerTransform.SetPosition(cData.hitPos.x, cData.hitPos.y, cData.hitPos.z);

        float velocityIntoSurface =
        glm::dot(playerVelocity, cData.normal);

        if (velocityIntoSurface < 0.0f)
        {
            glm::vec3 resolve = (cData.normal * 1.01f) * velocityIntoSurface;
            playerVelocity -= resolve;
        }
    }     
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
