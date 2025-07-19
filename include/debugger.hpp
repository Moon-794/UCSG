#ifndef U_DEBUGGER
#define U_DEBUGGER

#include "renderer.hpp"
#include "area.hpp"

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

struct DebuggerState
{
    bool showCollisionMap = false;
    bool showActiveColliders = false;
};

struct DebuggerInfo
{
    const Area& areaData;
    glm::vec2 playerPosition;
};

/* 
Developer tool to allow visualisation + manipulation of game systems.
Should not be included in release builds at this point. May in future become an in game console.
*/
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