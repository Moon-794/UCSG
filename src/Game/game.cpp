#include "Game/game.hpp"

Game::Game()
{
    Init();
}

void Game::Init()
{
    engine.Init();
    assetManager = std::make_unique<AssetManager>();

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
    if(engine.inputMap->GetKey(GLFW_KEY_W))
        cameraPos += cameraFront;
    
    if(engine.inputMap->GetKey(GLFW_KEY_S))
        cameraPos -= cameraFront;

    if(engine.inputMap->GetKey(GLFW_KEY_A))
        cameraPos -= cameraRight;
    
    if(engine.inputMap->GetKey(GLFW_KEY_D))
        cameraPos += cameraRight;

    if(engine.inputMap->GetKey(GLFW_KEY_SPACE))
        cameraPos += cameraUp;
    
    if(engine.inputMap->GetKey(GLFW_KEY_LEFT_SHIFT))
        cameraPos -= cameraUp;
}

void Game::Tick()
{
    //Update DeltaTime
    float currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - lastFrame;
    lastFrame = currentFrameTime;
}

void Game::Render()
{
    engine.renderer->Clear();

    //engine.renderer.DrawFrame

    glUseProgram(assetManager->GetShader("base")->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    //Camera Angle
    float yaw = engine.renderer->yaw;
    float pitch = engine.renderer->pitch;

    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
    direction.y = glm::sin(glm::radians(pitch));
    direction.z = glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));

    cameraFront = glm::normalize(glm::vec3(direction.x, 0.0f, direction.z));
    cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));

    int windowWidth;
    int windowHeight;
    glfwGetFramebufferSize(engine.renderer->window, &windowWidth, &windowHeight);

    engine.renderer->projection = glm::perspective(glm::radians(80.0f), (float)windowWidth / (float)windowHeight, 0.01f, 20000.0f);
    view = glm::lookAt(cameraPos, cameraPos + glm::normalize(direction), cameraUp);

    //These correspond to the 
    model = glm::translate(model, glm::vec3(0, 0, 0));
    model = glm::scale(model, glm::vec3(1, 1, 1));

    assetManager->GetShader("base")->setMat4("projection", engine.renderer->projection);
    assetManager->GetShader("base")->setMat4("model", model);
    assetManager->GetShader("base")->setMat4("view", view);

    glBindVertexArray(engine.renderer->chunkVAO);
    glActiveTexture(GL_TEXTURE0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(GL_TRIANGLES, 6 * 16 * 16, GL_UNSIGNED_INT, 0);
    
    engine.renderer->SwapBuffers();
    isRunning = !glfwWindowShouldClose(engine.renderer->window);
}

void Game::QuitGame()
{
    glfwTerminate();
    exit(0);
}