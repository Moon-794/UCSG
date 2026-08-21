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

    Transform& cameraTransform = engine.renderer->camera.transform;

    //Player controls
    if(engine.inputMap->GetKey(GLFW_KEY_W))
        cameraTransform.Translate(cameraTransform.Forward());
    
    if(engine.inputMap->GetKey(GLFW_KEY_S))
        cameraTransform.Translate(-cameraTransform.Forward());

    if(engine.inputMap->GetKey(GLFW_KEY_A))
        cameraTransform.Translate(cameraTransform.Right());
    
    if(engine.inputMap->GetKey(GLFW_KEY_D))
        cameraTransform.Translate(-cameraTransform.Right());
}

void Game::Tick()
{
    //Update DeltaTime
    float currentFrameTime = glfwGetTime();
    deltaTime = currentFrameTime - lastFrame;
    lastFrame = currentFrameTime;

    //Update Camera
}

void Game::Render()
{
    engine.renderer->Clear();

    //engine.renderer.DrawFrame

    glUseProgram(assetManager->GetShader("base")->ID);

    //Base uniforms, different shaders will likely have different uniforms
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);

    Transform cameraTransform = engine.renderer->camera.transform;
    glm::vec3 test = cameraTransform.Forward();

    view = glm::lookAt(cameraTransform.GetPosition(), cameraTransform.GetPosition() + cameraTransform.Forward(), cameraTransform.Up());
    
    //These correspond to the grid
    model = glm::translate(model, glm::vec3(0, 0, 0));
    model = glm::scale(model, glm::vec3(1, 1, 1));

    assetManager->GetShader("base")->setMat4("projection", engine.renderer->camera.GetProjection());
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