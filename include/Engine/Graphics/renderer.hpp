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

#include "Game/asteroid.hpp"

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
    void DrawAsteroid(const Asteroid& asteroid);
    void DrawDebugCube(glm::vec3 position, glm::vec3 scale, glm::vec3 color);

    //This stuff can eventually go away or somewhere else
    unsigned int chunkVAO;
    unsigned int quadVAO;
    unsigned int cubeVAO;
    
    float lastMouseX = 1280.0f;
    float lastMouseY = 720.0f;
private:
};

unsigned int CreateQuadVAO(); //! as all of these are primitives these are very low concern, probs just throw it in voxel manager or something
unsigned int CreateCubeVAO();
unsigned int CreateChunkVAO();

void DrawChunk();                                               //! Not doing the 2D chunks anymore so forget
void DrawSprite(Renderer& renderer, const Sprite& sprite);      //! Will def be doing sprites for UI later but not a concern right now

void framebuffer_size_callback(GLFWwindow* window, int width, int height);              //!idek if these can be put somewhere nice but they need to exists so meh
static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

#endif