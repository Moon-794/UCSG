#include <iostream>
#include "World.hpp"
#include "renderer.hpp"
#include "unistd.h"
#include <array>

class InputMap
{
public:

    int GetKey(int key)
    {
        return keyMap[key];
    }

    void SetKey(int key, int status)
    {
        keyMap[key] = status;
    }

private:
    std::array<int, 90> keyMap;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

int main(int argc, char** args)
{
    World world(16, 16);

    Renderer renderer;
    RendererSetupHints hints;
    hints.windowName = "Space Game";
    hints.windowWidth = 800;
    hints.windowHeight = 600;

    InitRenderer(renderer, hints);
    Shader s = Shader("resources/vertex.vert", "resources/fragment.frag");

    InputMap* inputMap = new InputMap();
    glfwSetWindowUserPointer(renderer.window, reinterpret_cast<void *>(inputMap));
    glfwSetKeyCallback(renderer.window, key_callback);

    Sprite tile;
    tile.textureID = GenerateTextureID("tile.png");
    tile.shaderID = s.ID;
    tile.x = 2;
    tile.y = 2;

    while (!glfwWindowShouldClose(renderer.window))
    {
        //Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawSprite(renderer, tile);

        if(inputMap->GetKey(GLFW_KEY_D) == 1)
        {
            renderer.cameraPos.x += 0.05f;
        }

        if(inputMap->GetKey(GLFW_KEY_A) == 1)
        {
            renderer.cameraPos.x -= 0.05f;
        }

        if(inputMap->GetKey(GLFW_KEY_W) == 1)
        {
            renderer.cameraPos.y += 0.05f;
        }

        if(inputMap->GetKey(GLFW_KEY_S) == 1)
        {
            renderer.cameraPos.y -= 0.05f;
        }
        
        //Finish Render pass
        glfwSwapBuffers(renderer.window);
        glfwPollEvents();
    }

    //Cleanup
    glfwTerminate();
    delete (inputMap);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    InputMap* map = reinterpret_cast<InputMap*>(glfwGetWindowUserPointer(window));

    if(action == GLFW_PRESS)
    {
        map->SetKey(key, 1);
        return;
    }

    if(action == GLFW_RELEASE)
    {
        map->SetKey(key, 0);
        return;
    }
}