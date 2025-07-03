#include "game.hpp"

Game::Game()
{
    Init();
}

void Game::Init()
{
    //Renderer setup
    this->renderer = std::make_unique<Renderer>("Space Game", 2560, 1440);    
    this->renderer->SetClearColor(0.1f, 0.1f, 0.1f, 0.1f);

    debugger = std::make_unique<Debugger>();
    debugger->InitImGUI(renderer->window);

    //Input setup Note: Relies on renderer, narsty
    this->inputMap = std::make_shared<InputMap>();
    glfwSetWindowUserPointer(renderer->window, reinterpret_cast<void*>(inputMap.get()));
    glfwSetKeyCallback(renderer->window, key_callback);

    //Resource Manager setup
    assetManager = std::make_unique<AssetManager>();

    //AreaManager Setup
    this->areaManager = std::make_unique<AreaManager>();
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
}

void Game::Tick()
{
    
}

void Game::Render()
{
    const AreaData& currentArea = areaManager->getCurrentArea();

    renderer->Clear();

    glUseProgram(assetManager->GetShader("base")->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(renderer->window, &windowWidth, &windowHeight);

    float vW = (float) windowWidth / 2;
    float vH = (float) windowHeight / 2;
    renderer->projection = glm::ortho(-vW, vW, -vH, vH, 0.1f, 15.0f);
    
    float camX = 64 * -renderer->cameraPos.x;
    float camY = 64 * renderer->cameraPos.y;

    view  = glm::translate(view, glm::vec3((float)0, (float)0, 0.0f));
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -1.0f));

    model = glm::translate(model, glm::vec3(0, 0, 0));
    model = glm::scale(model, glm::vec3(1 * 64, 1 * 64, 1));

    assetManager->GetShader("base")->setMat4("projection", renderer->projection);
    assetManager->GetShader("base")->setMat4("model", model);
    assetManager->GetShader("base")->setMat4("view", view);

    glBindVertexArray(renderer->chunkVAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, assetManager->GetTexture("wurmo"));

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6 * 16 * 16, GL_UNSIGNED_INT, 0);

    DebuggerInfo debugInfo = {currentArea, glm::vec2(0, 0)};
    debugger->DrawDebugger(*renderer, debugInfo);
    
    renderer->SwapBuffers();
    isRunning = !glfwWindowShouldClose(renderer->window);
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}