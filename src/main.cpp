#include <iostream>
#include "World.hpp"
#include "renderer.hpp"
#include "unistd.h"

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

    Sprite tile;
    tile.textureID = GenerateTextureID("tile.png");
    tile.shaderID = s.ID;
    tile.x = 4;
    tile.y = 4;

    while (!glfwWindowShouldClose(renderer.window))
    {
        //Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        DrawSprite(renderer, tile);
        
        //Finish Render pass
        glfwSwapBuffers(renderer.window);
        glfwPollEvents();
    }

    //Cleanup
    glfwTerminate();
}