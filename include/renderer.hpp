#ifndef U_RENDERER
#define U_RENDERER

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <GL/gl.h>
#include "sprite.hpp"
#include "shader.hpp"
#include <string>

class Renderer
{
public:
    Renderer(std::string windowName, int windowWidth, int windowHeight);

    GLFWwindow* window = nullptr;

    int windowWidth;
    int windowHeight;
    unsigned int quadVAO;

    glm::vec3 cameraPos = glm::vec3(0);
    glm::mat4 projection = glm::mat4(1.0f);

    void SwapBuffers();
    void Clear();
    void SetClearColor(float r, float g, float b, float a);

private:
};

unsigned int CreateQuadVAO();
void DrawSprite(Renderer& renderer, Sprite& sprite);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

#endif