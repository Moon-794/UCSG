#include "Engine/Graphics/renderer.hpp"
#include "Engine/engine.hpp"

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
    //Initialise GLFW
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
    chunkVAO = CreateChunkVAO();

    SetClearColor(0.1f, 0.1f, 0.1f, 0.1f);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(window);
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

unsigned int CreateChunkVAO()
{
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int width = 16;
    unsigned int height = 16;

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    //Generate a grid of quads x wide and y tall, quads do not share vertices to support UV indexing
    for (size_t x = 0; x < width; x++)
    {
        for (size_t y = 0; y < height; y++)
        {   
            //Quad vertices
            unsigned int index = static_cast<unsigned int>(vertices.size() / 5);
            float xPos = (float)x;
            float yPos = (float)y;

            vertices.insert(vertices.end(), {xPos + 1.0f,  yPos,            0.0f,   1.0f, 1.0f});         
            vertices.insert(vertices.end(), {xPos + 1.0f,  yPos - 1.0f,     0.0f,   1.0f, 0.0f}); 
            vertices.insert(vertices.end(), {xPos,         yPos - 1.0f,     0.0f,   0.0f, 0.0f});         
            vertices.insert(vertices.end(), {xPos,         yPos,            0.0f,   0.0f, 1.0f});
            
            indices.insert(indices.end(), {index, index + 1, index + 3, index + 1, index + 2, index + 3});
        }
    }

    std::cout << "Vertices: " << vertices.size() / 5 << "\n";
    std::cout << "Indices: " << indices.size() << "\n";

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
    
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    return VAO;
}