#ifndef U_RENDERER
#define U_RENDERER

#include "Third_Party/glad/glad.h"
#include "GLFW/glfw3.h"

#include <GL/gl.h>
#include <string>
#include <vector>
#include "stdbool.h"

#include "Engine/Graphics/sprite.hpp"
#include "Engine/Graphics/shader.hpp"
#include "Engine/Graphics/camera.hpp"
#include "Engine/Graphics/asset_manager.hpp"

class Renderer
{
public:
    Renderer(std::string windowName, int windowWidth, int windowHeight);

    // Can eventually go in a GameWindow Class or something
    GLFWwindow* window = nullptr;
    int windowWidth;
    int windowHeight;
    bool windowCloseRequest = false;
    //-------------------

    std::unique_ptr<AssetManager> assetManager;

    Camera camera;
    void SwapBuffers();
    void Clear();
    void SetClearColor(float r, float g, float b, float a);
    void Draw();

    //This stuff can eventually go away or somewhere else
    unsigned int chunkVAO;
    unsigned int quadVAO;
    float lastMouseX = 1280.0f;
    float lastMouseY = 720.0f;
private:
};

//idek what to do with these sort it out later

unsigned int CreateQuadVAO();
void DrawSprite(Renderer& renderer, const Sprite& sprite);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

unsigned int CreateChunkVAO();
void DrawChunk();

#endif