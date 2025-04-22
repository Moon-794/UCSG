#include <iostream>
#include "area.hpp"
#include "renderer.hpp"
#include "unistd.h"
#include <array>

#include <fstream> 
#include "collision.h"
#include <json-c/json.h>
#include "tilemap.h"

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
unsigned int GenerateMapTexture();

int main(int argc, char** args)
{
    Area area(std::string("TestMap"));
    

    Renderer renderer;
    RendererSetupHints hints;
    hints.windowName = "Space Game";
    hints.windowWidth = 800;
    hints.windowHeight = 600;

    InitRenderer(renderer, hints);
    Shader s = Shader("resources/vertex.vert", "resources/fragment.frag");

    InputMap* inputMap = new InputMap();
    glfwSetWindowUserPointer(renderer.window, reinterpret_cast<void*>(inputMap));
    glfwSetKeyCallback(renderer.window, key_callback);

    Sprite tile(std::string("tile.png"), glm::vec2(2, 2), &s);
    Sprite player(std::string("wurmo.png"), glm::vec2(0, 0), &s);

    unsigned int mapTex = GenerateMapTexture();
    Sprite map(mapTex, glm::vec2(0, 0), &s);
    map.scale = glm::vec2(32, 32);

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
        glClearColor(0.30f, 0.3f, 0.3f, 1.0f);
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
  
unsigned int GenerateMapTexture()
{
    json_object *data = GetDataArray("resources/areaData/TestMap/TestMap.json");
    int data_len = json_object_array_length(data);

    std::vector<int> tileIDS(32 * 32);
    int width = 32;
    int height = 32;
    int nrChannels = 0;
    int spriteWidth = 16;

    for (size_t i = 0; i < data_len; i++)
    {
        json_object *tile = json_object_array_get_idx(data, i);
        tileIDS[tileIDS.size() - 1 - i] = json_object_get_int(tile);
    }

    stbi_set_flip_vertically_on_load(true);
    stbi_uc* image = stbi_load("resources/areaData/TestMap/TestMap.png", &width, &height, &nrChannels, 4);
    int length = width * height * 4;

    int numImages = width / spriteWidth;
    
    std::vector<unsigned char*> subImages(numImages);

    for (size_t n = 0; n < numImages; n++)
    {
        subImages[n] = new unsigned char[16 * 16 * 4];
        for (size_t x = 0; x < 16; x++)
        {
            for (size_t y = 0; y < 16; y++)
            {
                int parentOffset = ((x * 80) + y + (spriteWidth * n)) * 4;
                int childOffset = ((x * 16) + y) * 4;
                subImages[n][childOffset + 0] = image[parentOffset + 0];
                subImages[n][childOffset + 1] = image[parentOffset + 1];
                subImages[n][childOffset + 2] = image[parentOffset + 2];
                subImages[n][childOffset + 3] = image[parentOffset + 3];
            }
        }
    }
    
    
    //Begin stitching together tilemap
    int mapWidth = 32;
    int mapHeight = 32;

    unsigned char* tileMapImage = new unsigned char[512 * 512 * 4];

    for (size_t x = 0; x < mapWidth; x++)
    {          
        for (size_t y = 0; y < mapHeight; y++)
        {
            int tileID = tileIDS[(x * mapWidth) + y];

            //Copy subImage to tilemapImage
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 0; j < 16; j++)
                {
                    //Move down 16 lines for each y
                    int mapOffsetY = ((x * 16) + i) * (16 * mapWidth * 4);
                    //Move down 1 line per j
                    int subOffsetY = i * 16 * 4;
                    
                    //Move 16 pixels across for each x
                    int mapOffsetX = y * (16 * 4);
                    int subOffsetX = j * 4;

                    int subOffset = subOffsetY + subOffsetX;
                    int mapOffset = mapOffsetY + mapOffsetX;

                    tileMapImage[mapOffset + subOffsetX + 0] = subImages[tileID - 1][subOffset + 0];
                    tileMapImage[mapOffset + subOffsetX + 1] = subImages[tileID - 1][subOffset + 1];
                    tileMapImage[mapOffset + subOffsetX + 2] = subImages[tileID - 1][subOffset + 2];
                    tileMapImage[mapOffset + subOffsetX + 3] = subImages[tileID - 1][subOffset + 3];
                }
            }
            
        }
    }

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, tileMapImage);

    //Free stb data

    //Delete subimages
    //Delete tilemapImage

    return texture;
}