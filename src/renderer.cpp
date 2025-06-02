#include "renderer.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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

Renderer::Renderer(std::string windowName, int windowWidth, int windowHeight)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int count;
    GLFWmonitor **monitors = glfwGetMonitors(&count);

    //Create Window
    window = glfwCreateWindow(windowWidth, windowHeight, "Space Game", nullptr, nullptr);
    if(window == nullptr)
    {
        std::cout << "Error: Failed to Create GLFW window.\n";
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1);

    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    //Load GLAD (needs a context)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    int viewX = windowWidth / 2;

    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_BLEND);

    quadVAO = CreateQuadVAO();

    float tilesVertically = 10.0f;
    float aspectRatio = (float)windowWidth / (float)windowHeight;

    //Move into its own callback for window resizing
    float vW = (float)windowWidth / 2;
    float vH = (float)windowHeight / 2;
    projection = glm::ortho(-vW, vW, -vH, vH, 0.1f, 15.0f);
}

void DrawSprite(Renderer& renderer, Sprite& sprite)
{
    glUseProgram(sprite.shader->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(renderer.window, &windowWidth, &windowHeight);

    float vW = (float)windowWidth / 2;
    float vH = (float)windowHeight / 2;
    renderer.projection = glm::ortho(-vW, vW, -vH, vH, 0.1f, 15.0f);
    
    float camX = 64 * -renderer.cameraPos.x;
    float camY = 64 * renderer.cameraPos.y;

    view  = glm::translate(view, glm::vec3((float)camX, (float)camY, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));

    model = glm::translate(model, glm::vec3((sprite.position.x * 64), -(sprite.position.y * 64), 0));
    model = glm::scale(model, glm::vec3(sprite.scale.x * 64, sprite.scale.y * 64, 1));

    sprite.shader->setMat4("projection", renderer.projection);
    sprite.shader->setMat4("model", model);
    sprite.shader->setMat4("view", view);

    glBindVertexArray(renderer.quadVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sprite.textureID);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
    glFinish();
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b,a);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT);
}