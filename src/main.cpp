#include <iostream>
#include "World.hpp"
#include "renderer.hpp"
#include "unistd.h"
#include <array>

#include "box2d/box2d.h"

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
    b2Polygon box = b2MakeBox(1.0f, 1.0f);

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

    Sprite tile(std::string("tile.png"), glm::vec2(2, 2), &s);
    Sprite player(std::string("wurmo.png"), glm::vec2(0, 0), &s);
    Sprite map(std::string("map.png"), glm::vec2(0, 0), &s);
    map.scale.x = 16;
    map.scale.y = 16;

    while (!glfwWindowShouldClose(renderer.window))
    {
        //Clear screen
        glClearColor(0.090f, 0.098f, 0.117f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawSprite(renderer, map);
        DrawSprite(renderer, player);

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

        player.position.x = renderer.cameraPos.x;
        player.position.y = renderer.cameraPos.y;
        
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