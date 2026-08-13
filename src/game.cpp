#include "game.hpp"

Game::Game()
{
    Init();
}

void Game::Init()
{
    this->renderer = std::make_unique<Renderer>("Space Game", 2560, 1440);    
    this->renderer->SetClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    debugger = std::make_unique<Debugger>();
    debugger->InitImGUI(renderer->window);

    this->inputMap = std::make_shared<InputMap>();
    glfwSetWindowUserPointer(renderer->window, reinterpret_cast<void*>(inputMap.get()));
    glfwSetKeyCallback(renderer->window, key_callback);

    assetManager = std::make_unique<AssetManager>();

    //Make a big ol voxel, 3D now i guess

    Run();
}

void Game::Run()
{
    while(isRunning)
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        UpdateInputs();
        Tick();
        Render();

        auto frameEnd = std::chrono::high_resolution_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart);
        auto timeLeft = std::chrono::microseconds(16670) - frameDuration;

        while (std::chrono::high_resolution_clock::now() < frameEnd + timeLeft) 
        {
            // Just spinning
        }
    }

    QuitGame();
}

void Game::UpdateInputs()
{
    glfwPollEvents();

    //Player controls
    if(inputMap->GetKey(GLFW_KEY_W))
        playerz += 1;
    
    if(inputMap->GetKey(GLFW_KEY_S))
        playerz -= 1;

    if(inputMap->GetKey(GLFW_KEY_A))
        playerx += 1;
    
    if(inputMap->GetKey(GLFW_KEY_D))
        playerx -= 1;

    if(inputMap->GetKey(GLFW_KEY_SPACE))
        playery -= 1;
    
    if(inputMap->GetKey(GLFW_KEY_LEFT_SHIFT))
        playery += 1;

    //Camera Controls
    if(inputMap->GetKey(GLFW_KEY_LEFT))
        cameraX -= 1;
    
    if(inputMap->GetKey(GLFW_KEY_RIGHT))
        cameraX += 1;

    if(inputMap->GetKey(GLFW_KEY_UP))
        cameraZ += 1;
    
    if(inputMap->GetKey(GLFW_KEY_DOWN))
        cameraZ -= 1;

    //Mouse Controls
    glfwGetCursorPos(renderer->window, &cameraX, &cameraZ);
}

void Game::Tick()
{
    
}

void Game::Render()
{
    renderer->Clear();

    glUseProgram(assetManager->GetShader("base")->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(renderer->window, &windowWidth, &windowHeight);

    renderer->projection = glm::perspective(glm::radians(80.0f), (float)windowWidth / (float)windowHeight, 0.01f, 20000.0f);
    float t = glfwGetTime();

    view = glm::rotate(view, glm::radians((float)cameraX), glm::vec3(0.0f, 1.0f, 0.0f));
    view  = glm::translate(view, glm::vec3(playerx, playery, playerz));

    model = glm::translate(model, glm::vec3(0, 0, 0));
    model = glm::scale(model, glm::vec3(1 * 64, 1 * 64, 1));

    assetManager->GetShader("base")->setMat4("projection", renderer->projection);
    assetManager->GetShader("base")->setMat4("model", model);
    assetManager->GetShader("base")->setMat4("view", view);

    //std::cout << playerz << std::endl;

    glBindVertexArray(renderer->chunkVAO);

    glActiveTexture(GL_TEXTURE0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6 * 16 * 16, GL_UNSIGNED_INT, 0);
    
    renderer->SwapBuffers();
    isRunning = !glfwWindowShouldClose(renderer->window);
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}