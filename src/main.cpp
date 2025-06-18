#define STB_IMAGE_IMPLEMENTATION

#include "game.hpp"
#include "resource_manager.hpp"
#include "renderer.hpp"
#include "debugger.hpp"
#include "area.hpp"
#include "input.h"
#include "collision.hpp"

#include "unistd.h"
#include <iostream>
#include <array>
#include <thread>
#include <chrono>

//This is getting very messy very quickly, some things are going to have to be addressed
//before moving on.
//
// 1 - Everything is in the main function, this sucks
// 2 - Data and rendering is tightly coupled, sprites are both data + graphics, this is also
// bad.
// 3 - AreaManager holds collision data, this wont scale well if the game SCENE also has entities
// that can be collided with
// 4 - The current collision algorithm just doesnt work

//Current solutions
//1 - Start moving everything into the Game class

//this is actually so hard lol

int main(int argc, char** args)
{
    Renderer renderer("Space Game", 2560, 1440);
    renderer.SetClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    ResourceManager resourceManager;

    std::shared_ptr<Shader> baseShader = resourceManager.GetShader(std::string("base"));
    Debugger debugger(&renderer);
    InputMap* inputMap = new InputMap();

    glfwSetWindowUserPointer(renderer.window, reinterpret_cast<void*>(inputMap));
    glfwSetKeyCallback(renderer.window, key_callback);

    Sprite player(std::string("wurmo.png"), glm::vec2(0, 0), baseShader);
    player.scale = glm::vec2(1, 1);
    player.position = glm::vec2(16, 16);

    float playerSpeed = 0.1f;
    double currentTime = 0.0f;
    double previousTime = 0.0f;
    double timeDiff = 0.0f;
    unsigned int counter = 0;

    URect playerRect = {16, 16, 1, 1};
    URect tileRect = {0, 0, 1, 1}; 

    AreaManager areaManager(baseShader);
    
    bool keyDown = false;
    bool hallway = true;
 
    while (!glfwWindowShouldClose(renderer.window))
    {
        const AreaData& currentArea = areaManager.getCurrentArea();
        float moveX = 0.0f;
        float moveY = 0.0f;

        auto frameStart = std::chrono::high_resolution_clock::now();

        currentTime = glfwGetTime();
        timeDiff = currentTime - previousTime;
        counter++;

        if(timeDiff >= 1.0f / 60.0f)
        {
            std::string FPS = std::to_string((1.0f/timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle = "UCSG - " + FPS + " FPS -- " + ms + " ms";

            glfwSetWindowTitle(renderer.window, newTitle.c_str());
            previousTime = currentTime;
            counter = 0;
        }

        if(inputMap->GetKey(GLFW_KEY_D) == 1)
            moveX += playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_A) == 1)
            moveX -= playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_W) == 1)
            moveY -= playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_S) == 1)
            moveY += playerSpeed;

        player.position.x += moveX;
        player.position.y += moveY;
        playerRect.x = player.position.x;
        playerRect.y = player.position.y;

        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {   
                tileRect.x = std::floor(player.position.x + 0.5f + x);
                tileRect.y = std::floor(player.position.y + 0.5f + y);

                if(tileRect.x >= 0 && tileRect.y >= 0)
                {
                    int tileID = currentArea.tileLayers[0].layerData[tileRect.x][tileRect.y];

                    if(currentArea.tileLayers[0].collisionMap.at(tileID) == true)
                    {
                        if(AABBIntersect(playerRect, tileRect))
                        {
                            float overlapX1 = (tileRect.x + tileRect.width) - playerRect.x;
                            float overlapX2 = (playerRect.x + playerRect.width) - tileRect.x;

                            float resolveX = (overlapX1 < overlapX2) ? -overlapX1 : overlapX2;

                            player.position.x -= resolveX;
                            playerRect.x = player.position.x;
                        }
                        
                        if(AABBIntersect(playerRect, tileRect))
                        {
                            float overlapY1 = (tileRect.y + tileRect.height) - playerRect.y;
                            float overlapY2 = (playerRect.y + playerRect.height) - tileRect.y;
                            float resolveY = (overlapY1 < overlapY2) ? -overlapY1 : overlapY2;

                            player.position.y -= resolveY;
                            playerRect.y = player.position.y;
                        }
                    }
                }
            }   
        }

        renderer.cameraPos.x = player.position.x;
        renderer.cameraPos.y = player.position.y;

        renderer.Clear();

        DrawAreaLayer(renderer, currentArea, 0);
        DrawSprite(renderer, player);
        
        DebuggerInfo debugInfo = 
        {
            currentArea,
            player.position
        };

        debugger.DrawDebugger(debugInfo);

        renderer.SwapBuffers();

        if(inputMap->GetKey(GLFW_KEY_F) == 1 && keyDown == false)
        {
            keyDown = true;
            hallway = !hallway;

            if(hallway)
            {
                areaManager.ForceTransition("Hallway", glm::vec2(0, 16), player.position.x, player.position.y);
            }
            else
            {
                areaManager.ForceTransition("TestMap", glm::vec2(30, 13), player.position.x, player.position.y);
            }
        }
        else if(inputMap->GetKey(GLFW_KEY_F) == 0)
        {
            keyDown = false;
        }

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        auto timeLeft = std::chrono::microseconds(16670) - frameDuration;

        while (std::chrono::high_resolution_clock::now() < frameEnd + timeLeft) 
        {
            // Just spinnin
        }
    }

    //Cleanup
    glfwTerminate();
    delete (inputMap);
}