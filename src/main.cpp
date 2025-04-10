#include <iostream>
#include "area.hpp"
#include "renderer.hpp"
#include "unistd.h"
#include <array>

#include "collision.h"
#include <json-c/json.h>

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
    Area area(std::string("TestMap"));

    json_object *root = json_object_from_file("resources/areaData/TestMap/map.json");

    json_object *layers;
    json_object_object_get_ex(root, "layers", &layers);
    int array_len = json_object_array_length(layers);

    for (size_t i = 0; i < array_len; i++)
    {
        json_object *elem = json_object_array_get_idx(layers, i);
        json_object *name;
        json_object_object_get_ex(elem, "name", &name);
        printf("Name: %s\n", json_object_get_string(name));
    }

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

    URect playerRect;
    playerRect.x = 0;
    playerRect.y = 0;
    playerRect.width = 0.8;
    playerRect.height = 1;

    URect tileRect;
    tileRect.x = 2;
    tileRect.y = 2;
    tileRect.width = 1;
    tileRect.height = 1;

    while (!glfwWindowShouldClose(renderer.window))
    {
        //Clear screen
        glClearColor(0.090f, 0.098f, 0.117f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawSprite(renderer, map);
        DrawSprite(renderer, tile);
        DrawSprite(renderer, player);

        if(inputMap->GetKey(GLFW_KEY_D) == 1)
        {
            player.position.x += 0.05f;
        }

        if(inputMap->GetKey(GLFW_KEY_A) == 1)
        {
            player.position.x -= 0.05f;
        }

        if(inputMap->GetKey(GLFW_KEY_W) == 1)
        {
            player.position.y += 0.05f;
        }

        if(inputMap->GetKey(GLFW_KEY_S) == 1)
        {
            player.position.y -= 0.05f;
        }

        playerRect.x = player.position.x;
        playerRect.y = player.position.y;

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
            } else 
            {
                player.position.y -= resolveY;
            }

            playerRect.x = player.position.x;
            playerRect.y = player.position.y;
        }

        renderer.cameraPos.x = player.position.x;
        renderer.cameraPos.y = player.position.y;
        
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