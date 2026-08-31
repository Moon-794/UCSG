#include "Engine/Graphics/renderer.hpp"
#include "Engine/engine.hpp"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <string_view>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

unsigned int CreateCubeVAO()
{
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);   
    glBufferData(GL_ARRAY_BUFFER, sizeof(MeshConstants::cubeVertices), MeshConstants::cubeVertices, GL_DYNAMIC_DRAW);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(MeshConstants::cubeIndices), MeshConstants::cubeIndices, GL_DYNAMIC_DRAW);

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

    glViewport(0, 0, windowWidth, windowHeight);
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_CULL_FACE);      
    glFrontFace(GL_CCW); 

    cubeVAO = CreateCubeVAO();

    SetClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    //Init camera stuff
    camera.SetFOV(80.0f);
    camera.SetAspectRatio((float)windowWidth/(float)windowHeight);

    //Init AssetManager
    assetManager = std::make_unique<AssetManager>();

    shipMesh.Init();
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(window);
    windowCloseRequest = glfwWindowShouldClose(window);
    glFinish();
}

void Renderer::SetClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b,a);
}

void Renderer::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void Renderer::DrawAsteroid(const Asteroid& a)
{
    if(!a.isActive)
        return;
    
    glUseProgram(assetManager->GetShader("base")->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    glm::vec3 cameraRot = camera.transform.GetRotation();

    Transform cameraTransform = camera.transform;
    glm::vec3 cameraPos = cameraTransform.GetPosition();
    glm::vec3 cameraForward = cameraTransform.Forward();
    view = glm::lookAtLH(cameraPos, cameraPos + cameraForward, cameraTransform.Up());

    //These correspond to the grid
    model = glm::translate(model, a.transform.GetPosition());
    model = glm::scale(model, glm::vec3(1, 1, 1));

    assetManager->GetShader("base")->setMat4("projection", camera.GetProjection());
    assetManager->GetShader("base")->setMat4("model", model);
    assetManager->GetShader("base")->setMat4("view", view);

    glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);

    if(a.materialType == "iron")
        color = glm::vec3(51.0f/255.0f, 163.0f/255.0f, 232.0f/255.0f);
    else
        color = glm::vec3(230.0f/255.0f, 78.0f/255.0f, 14.0f/255.0f);
    
    assetManager->GetShader("base")->setVec3("inputColor", color);

    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Renderer::DrawDebugCube(glm::vec3 position, glm::vec3 scale, glm::vec3 color)
{
    glUseProgram(assetManager->GetShader("base")->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    Transform cameraTransform = camera.transform;
    glm::vec3 cameraPos = cameraTransform.GetPosition();
    glm::vec3 cameraForward = cameraTransform.Forward();
    view = glm::lookAt(cameraPos, cameraPos + cameraForward, cameraTransform.Up());

    //These correspond to the grid
    model = glm::translate(model, position);
    model = glm::scale(model, scale);

    assetManager->GetShader("base")->setMat4("projection", camera.GetProjection());
    assetManager->GetShader("base")->setMat4("model", model);
    assetManager->GetShader("base")->setMat4("view", view);

    assetManager->GetShader("base")->setVec3("inputColor", color);

    glBindVertexArray(cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Renderer::UpdateShipMesh(World& world)
{
    shipMesh.vertices.clear();
    shipMesh.indices.clear();

    for (size_t i = 0; i < world.ROWS; i++)
    {
        for (size_t j = 0; j < world.COLS; j++)
        {
            if(world.shipGrid[i][j] == TileType::ship)
            {
                // Floor and roof mesh
                unsigned int k = shipMesh.vertices.size() / 5;

                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 0.0f, j + 0.0f,    0.00f, 0.00f});
                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 0.0f, j + 1.0f,    0.25f, 0.25f});
                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 0.0f, j + 1.0f,    0.00f, 0.25f});
                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 0.0f, j + 0.0f,    0.25f, 0.00f});
                
                shipMesh.indices.insert(shipMesh.indices.end(), {k, k + 1, k + 2, k + 1, k, k + 3});
                k = shipMesh.vertices.size() / 5;

                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 4.0f, j + 0.0f,    0.25f, 0.00f});
                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 4.0f, j + 1.0f,    0.00f, 0.25f});
                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 4.0f, j + 1.0f,    0.25f, 0.25f});
                shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 4.0f, j + 0.0f,    0.00f, 0.00f});
                shipMesh.indices.insert(shipMesh.indices.end(), {k + 2, k + 1, k, k + 3, k, k + 1});

                //~-------------- WALLS -------------
                float rowRight = (i + 1) < world.ROWS ? i + 1 : world.ROWS - 1;

                float colDown = (j - 1) >= 0 ? j - 1: 0;
                float colUp = (j + 1) < world.COLS ? j + 1 : world.COLS - 1;

                if(i == 0 || world.shipGrid[i - 1][j] == TileType::empty)
                {
                    k = shipMesh.vertices.size() / 5;

                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i, 0.0f, j + 0.0f,    0.25f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i, 0.0f, j + 1.0f,    0.50f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i, 4.0f, j + 0.0f,    0.25f, 1.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i, 4.0f, j + 1.0f,    0.50f, 1.00f});
                    shipMesh.indices.insert(shipMesh.indices.end(), {k, k + 3, k + 2, k, k + 1, k + 3});
                }
                
                if(i == world.ROWS - 1 || world.shipGrid[i + 1][j] == TileType::empty)
                {
                    k = shipMesh.vertices.size() / 5;

                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i + 1, 0.0f, j + 0.0f,    0.50f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i + 1, 0.0f, j + 1.0f,    0.25f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i + 1, 4.0f, j + 0.0f,    0.50f, 1.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {(float)i + 1, 4.0f, j + 1.0f,    0.25f, 1.00f});
                    shipMesh.indices.insert(shipMesh.indices.end(), {k + 2, k + 3, k, k + 3, k + 1, k});
                }
                
                if(j == 0 || world.shipGrid[i][j - 1] == TileType::empty)
                {
                    k = shipMesh.vertices.size() / 5;

                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 0.0f, j + 0.0f,    0.50f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 0.0f, j + 0.0f,    0.25f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 4.0f, j + 0.0f,    0.50f, 1.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 4.0f, j + 0.0f,    0.25f, 1.00f});
                    shipMesh.indices.insert(shipMesh.indices.end(), {k + 2, k + 3, k, k + 3, k + 1, k});
                }

                if(j == world.COLS - 1 || world.shipGrid[i][j + 1] == TileType::empty)
                {
                    k = shipMesh.vertices.size() / 5;

                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 0.0f, j + 1.0f,    0.25f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 0.0f, j + 1.0f,    0.50f, 0.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 0.0f, 4.0f, j + 1.0f,    0.25f, 1.00f});
                    shipMesh.vertices.insert(shipMesh.vertices.end(), {i + 1.0f, 4.0f, j + 1.0f,    0.50f, 1.00f});
                    shipMesh.indices.insert(shipMesh.indices.end(), {k, k + 3, k + 2, k, k + 1, k + 3});
                }
            }
            
        }
        
    }

    shipMesh.UpdateBuffers();
}

void Renderer::DrawShip()
{
    glUseProgram(assetManager->GetShader("area")->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    Transform cameraTransform = camera.transform;
    glm::vec3 cameraPos = cameraTransform.GetPosition();
    glm::vec3 cameraForward = cameraTransform.Forward();
    view = glm::lookAtLH(cameraPos, cameraPos + cameraForward, cameraTransform.Up());

    //These correspond to the grid
    model = glm::translate(model, glm::vec3(0, 0, 0));
    model = glm::scale(model, glm::vec3(1, 1, 1));

    assetManager->GetShader("area")->setMat4("projection", camera.GetProjection());
    assetManager->GetShader("area")->setMat4("model", model);
    assetManager->GetShader("area")->setMat4("view", view);

    assetManager->GetShader("area")->setVec3("inputColor", glm::vec3(1.0f, 1.0f, 1.0f));
    unsigned int texID = assetManager->GetTexture("Textures");
    glBindVertexArray(shipMesh.VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texID);
    glPolygonMode(GL_FRONT, GL_FILL);
    glDrawElements(GL_TRIANGLES, 32 * 32 * 6 * 2 * 4, GL_UNSIGNED_INT, 0);
}