#ifndef U_ENGINE
#define U_ENGINE

#include "Engine/input.h"
#include "Engine/Graphics/renderer.hpp"
#include "Engine/Debugger/debugger.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Engine
{
public:
    Engine();

    void Init();
    
    std::shared_ptr<InputMap> inputMap;
    std::unique_ptr<Renderer> renderer;

private:

};

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void cursor_position_callback(GLFWwindow* window, double xPos, double yPos);

#endif