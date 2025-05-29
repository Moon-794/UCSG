#define STB_IMAGE_IMPLEMENTATION

#include "renderer.hpp"
#include "debugger.hpp"
#include "area.hpp"
#include "input.h"
#include "collision.h"

#include <iostream>
#include "unistd.h"
#include <array>
#include <thread>
#include <chrono>

int main(int argc, char** args)
{
    Renderer renderer("Space Game", 2560, 1440);
    Shader s = Shader("resources/shaders/base/vertex.vert", "resources/shaders/base/fragment.frag");
    Debugger debugger(&renderer);

    Area area(std::string("TestMap"));
    area.SetShader(&s);

    InputMap* inputMap = new InputMap();

    glfwSetWindowUserPointer(renderer.window, reinterpret_cast<void*>(inputMap));
    glfwSetKeyCallback(renderer.window, key_callback);

    Sprite player(std::string("wurmo.png"), glm::vec2(0, 0), &s);
    player.scale = glm::vec2(1, 1);
    player.position = glm::vec2(16, 16);

    float playerSpeed = 0.1f;
    double currentTime = 0.0f;
    double previousTime = 0.0f;
    double timeDiff = 0.0f;
    unsigned int counter = 0;

    DebuggerInfo debugInfo;
    debugInfo.area = &area;

    URect playerRect = {16, 16, 1, 1};
    URect tileRect = {0, 0, 1, 1};

    while (!glfwWindowShouldClose(renderer.window))
    {
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

        glClearColor(0.30f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        area.DrawLayer(renderer, 0);
        DrawSprite(renderer, player);
        area.DrawLayer(renderer, 1);

        debugInfo.playerPosition = player.position;
        debugger.DrawDebugger(debugInfo);

        glfwSwapBuffers(renderer.window);
        glfwPollEvents();
        DwmFlush();
        glFinish();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        auto timeLeft = std::chrono::microseconds(16670) - frameDuration;

        while (std::chrono::high_resolution_clock::now() < frameEnd + timeLeft) 
        {
            // Just spinning
        }
    }

    //Cleanup
    glfwTerminate();
    delete (inputMap);
}