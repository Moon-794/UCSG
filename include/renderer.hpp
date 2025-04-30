#ifndef U_RENDERER
#define U_RENDERER

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <windows.h>
#include <GL/gl.h>
#include "dwmapi.h"

#include "sprite.hpp"
#include "shader.hpp"
#include <string>

struct RendererSetupHints
{
    std::string windowName;
    int windowWidth;
    int windowHeight;
};

struct Renderer
{
public:
    GLFWwindow* window = nullptr;
    int windowWidth;
    int windowHeight;
    unsigned int quadVAO;
    glm::vec3 cameraPos = glm::vec3(0);
    glm::mat4 projection = glm::mat4(1.0f);
};

unsigned int CreateQuadVAO()
{
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);   
    glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteConstants::vertices), SpriteConstants::vertices, GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SpriteConstants::indices), SpriteConstants::indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}

int InitRenderer(Renderer& renderer, RendererSetupHints& hints)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);

    //Create Window
    renderer.window = glfwCreateWindow(hints.windowWidth, hints.windowHeight, "Space Game", nullptr, nullptr);
    if(renderer.window == nullptr)
    {
        std::cout << "Error: Failed to Create GLFW window.\n";
        return -1;
    }

    glfwMakeContextCurrent(renderer.window);
    glfwSwapInterval(1);

    renderer.windowWidth = hints.windowWidth;
    renderer.windowHeight = hints.windowHeight;

    //Load GLAD (needs a context)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    int viewX = hints.windowWidth / 2;

    glViewport(0, 0, hints.windowWidth, hints.windowHeight);
    glEnable(GL_BLEND);

    renderer.quadVAO = CreateQuadVAO();

    //Move into its own callback for window resizing
    float vW = (float)renderer.windowWidth / 2;
    float vH = (float)renderer.windowHeight / 2;
    renderer.projection = glm::ortho(-vW, vW, -vH, vH, 0.1f, 15.0f);

    return 0;
}

void DrawSprite(Renderer& renderer, Sprite& sprite)
{
    glUseProgram(sprite.shader->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    

    int camX = -renderer.cameraPos.x;
    int camY = -renderer.cameraPos.y;

    view  = glm::translate(view, glm::vec3((float)camX, (float)camY, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));

    model = glm::translate(model, glm::vec3(sprite.position.x, sprite.position.y, 0));
    model = glm::scale(model, glm::vec3(sprite.scale, 1));

    sprite.shader->setMat4("projection", renderer.projection);
    sprite.shader->setMat4("model", model);
    sprite.shader->setMat4("view", view);

    glBindVertexArray(renderer.quadVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.textureID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

#endif