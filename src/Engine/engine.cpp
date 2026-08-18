#include "Engine/engine.hpp"

Engine::Engine()
{
    //Do nothing
}

void Engine::Init()
{
    renderer = std::make_unique<Renderer>("Space Game", 2560, 1440);    
    inputMap = std::make_shared<InputMap>();

    glfwSetWindowUserPointer(renderer->window, reinterpret_cast<void*>(this));
    glfwSetFramebufferSizeCallback(renderer->window, framebuffer_size_callback);
    glfwSetKeyCallback(renderer->window, key_callback);
    glfwSetCursorPosCallback(renderer->window, cursor_position_callback);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Engine* engine = reinterpret_cast<Engine*>(glfwGetWindowUserPointer(window));

    if(action == GLFW_PRESS)
    {
        engine->inputMap->SetKey(key, 1);
        return;
    }

    if(action == GLFW_RELEASE)
    {
        engine->inputMap->SetKey(key, 0);
        return;
    }

    
    std::cout << "CURSOR CALLBACK ADDR: " << engine->renderer.get() << "\n";
}

void cursor_position_callback(GLFWwindow* window, double xPos, double yPos)
{
    Engine* engine = static_cast<Engine*>(glfwGetWindowUserPointer(window));

    float xOffset = xPos - engine->renderer->lastMouseX;
    float yOffset = engine->renderer->lastMouseY - yPos;

    const float sensitivity = 0.10f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    engine->renderer->yaw += xOffset;
    engine->renderer->pitch += yOffset;

    if(engine->renderer->pitch > 89.0f)
    engine->renderer->pitch =  89.0f;
    if(engine->renderer->pitch < -89.0f)
    engine->renderer->pitch = -89.0f;

    engine->renderer->lastMouseX = xPos;
    engine->renderer->lastMouseY = yPos;
}