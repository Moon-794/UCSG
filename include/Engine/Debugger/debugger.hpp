#ifndef U_DEBUGGER
#define U_DEBUGGER

#include "Engine/Graphics/renderer.hpp"

#include <Third_Party/imgui/imgui.h>
#include <Third_Party/imgui/imgui_impl_opengl3.h>
#include <Third_Party/imgui/imgui_impl_glfw.h>

struct DebuggerState
{
    bool showCollisionMap = false;
    bool showActiveColliders = false;
};

struct DebuggerInfo
{
    glm::vec2 playerPosition;
};

class Debugger
{
public:
    Debugger();
    void InitImGUI(GLFWwindow* window);

    void DrawDebugger(Renderer& renderer, DebuggerInfo info);
    void ProcessDebugger(Renderer& renderer, DebuggerInfo info);

private:
    std::shared_ptr<Shader> debugShader;
    Sprite debugSprite;
    DebuggerState state;
};

#endif