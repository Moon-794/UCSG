#include "debugger.hpp"

Debugger::Debugger()
{
    debugShader = std::make_shared<Shader>("debugger", "resources/shaders/debugger/vertex.vert", "resources/shaders/debugger/fragment.frag");
    debugSprite = Sprite("d_selector.png", glm::vec2(0, 0), debugShader);
    debugSprite.scale = glm::vec2(1, 1);
}

void Debugger::InitImGUI(GLFWwindow* window)
{
    //Enable ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Debugger::DrawDebugger(Renderer& renderer, DebuggerInfo info)
{
    ProcessDebugger(renderer, info);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debugger");
    int roundedX = std::round(info.playerPosition.x * 100.0f);
    int roundedY = std::round(info.playerPosition.y * 100.0f);

    std::string playerPosText("Player Position: [" + std::to_string(roundedX) + " - " + std::to_string(roundedY) + "]");
    ImGui::Text(playerPosText.c_str());

    ImGui::Checkbox("Show Active Colliders", &state.showActiveColliders);

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Debugger::ProcessDebugger(Renderer& renderer, DebuggerInfo info)
{
    if(state.showActiveColliders)
    {
        
    }
}
