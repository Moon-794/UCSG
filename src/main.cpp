#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include "renderer.hpp"
#include "area.hpp"
#include "unistd.h"
#include <array>

#include <thread>
#include <chrono>
#include "input.h"
#include "collision.h"
#include <json-c/json.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

int main(int argc, char** args)
{
    Renderer renderer("Space Game", 2560, 1440);
    Shader s = Shader("resources/vertex.vert", "resources/fragment.frag");

    Area area(std::string("TestMap"));
    area.SetShader(&s);

    InputMap* inputMap = new InputMap();
    glfwSetWindowUserPointer(renderer.window, reinterpret_cast<void*>(inputMap));
    glfwSetKeyCallback(renderer.window, key_callback);

    Sprite player(std::string("wurmo.png"), glm::vec2(0, 0), &s);
    player.scale = glm::vec2(64, 64);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(renderer.window, true);
    ImGui_ImplOpenGL3_Init("#version 460");

    float playerSpeed = 4.0f;
    double currentTime = 0.0f;
    double previousTime = 0.0f;
    double timeDiff = 0.0f;
    unsigned int counter = 0;

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

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Hello, world!");   
        ImGui::Text("This is some useful text.");  
        ImGui::End();

        

        if(inputMap->GetKey(GLFW_KEY_D) == 1)
            player.position.x += playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_A) == 1)
            player.position.x -= playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_W) == 1)
            player.position.y += playerSpeed;

        if(inputMap->GetKey(GLFW_KEY_S) == 1)
            player.position.y -= playerSpeed;

        renderer.cameraPos.x = glm::round(player.position.x);
        renderer.cameraPos.y = glm::round(player.position.y);

        glClearColor(0.30f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        area.DrawLayer(renderer, 0);
        DrawSprite(renderer, player);
        area.DrawLayer(renderer, 1);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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

/*if(PlayerAABBIntersect(playerRect, tileRect))
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
            } else 
            {
                player.position.y -= resolveY;
            }

            playerRect.x = player.position.x;
            playerRect.y = player.position.y;
        }*/