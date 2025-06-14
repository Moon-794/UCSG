#define STB_IMAGE_IMPLEMENTATION

#include "renderer.hpp"
#include "debugger.hpp"
#include "area.hpp"
#include "input.h"
#include "collision.h"

#include "unistd.h"
#include <iostream>
#include <array>
#include <thread>
#include <chrono>

//Next todo
//Turn std::shared_ptr<AreaData> to hold a container instead of the direct data
//Collision map/collision checker
//Get new areamanager working with debugger
//Area transition!

//Shader Manager within renderer to avoid having to pass around?
//Improve renderer in general to allow drawing of parented objects etc

//Gameobject structure might be useful soon, 

int main(int argc, char** args)
{
    Renderer renderer("Space Game", 2560, 1440);
    renderer.SetClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    std::shared_ptr<Shader> s = std::make_shared<Shader>("resources/shaders/base/vertex.vert", "resources/shaders/base/fragment.frag");
    Debugger debugger(&renderer);
    InputMap* inputMap = new InputMap();

    glfwSetWindowUserPointer(renderer.window, reinterpret_cast<void*>(inputMap));
    glfwSetKeyCallback(renderer.window, key_callback);

    Sprite player(std::string("wurmo.png"), glm::vec2(0, 0), s);
    player.scale = glm::vec2(1, 1);
    player.position = glm::vec2(16, 16);

    float playerSpeed = 0.1f;
    double currentTime = 0.0f;
    double previousTime = 0.0f;
    double timeDiff = 0.0f;
    unsigned int counter = 0;

    URect playerRect = {16, 16, 1, 1};
    URect tileRect = {0, 0, 1, 1}; 

    AreaManager areaManager(s);
    
    bool keyDown = false;
    bool hallway = true;
 
    while (!glfwWindowShouldClose(renderer.window))
    {
        const AreaData& currentArea = areaManager.getCurrentArea();

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
            player.position.x += playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_A) == 1)
            player.position.x -= playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_W) == 1)
            player.position.y -= playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_S) == 1)
            player.position.y += playerSpeed;

        playerRect.x = player.position.x;
        playerRect.y = player.position.y;

        renderer.cameraPos.x = player.position.x;
        renderer.cameraPos.y = player.position.y;

        
        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {   
                tileRect.x = player.position.x + x;
                tileRect.y = player.position.y + y;

                if(tileRect.x >= 0 && tileRect.y >= 0)
                {
                    int tileID = currentArea.tileLayers[0].layerData[tileRect.x][tileRect.y];
                    
                    if(currentArea.tileLayers[0].collisionMap.at(tileID) == true)
                    {
                        if(PlayerAABBIntersect(playerRect, tileRect))
                        {
                            float overlapX1 = (tileRect.x + tileRect.width) - playerRect.x;
                            float overlapX2 = (playerRect.x + playerRect.width) - tileRect.x;

                            float overlapY1 = (tileRect.y + tileRect.height) - playerRect.y;
                            float overlapY2 = (playerRect.y + playerRect.height) - tileRect.y;

                            float resolveX = (overlapX1 < overlapX2) ? -overlapX1 : overlapX2;
                            float resolveY = (overlapY1 < overlapY2) ? -overlapY1 : overlapY2;

                            if (std::abs(resolveX) < std::abs(resolveY)) 
                            {
                                player.position.x -= resolveX;
                            } 
                            else 
                            {
                                player.position.y -= resolveY;
                            }

                            playerRect.x = player.position.x;
                            playerRect.y = player.position.y;
                        }   
                    }
                }
            }   
        }

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