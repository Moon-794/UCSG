#include "Engine/Debugger/debugger.hpp"

Debugger::Debugger()
{
    std::cout << "Debugger Initialised" << std::endl;
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

void Debugger::Draw(Renderer& renderer, DebuggerInfo info)
{
    ProcessDebugger(renderer, info);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debugger");

    int roundedX = std::floor(info.playerPosition.x);
    int roundedY = std::floor(info.playerPosition.y);
    int roundedZ = std::floor(info.playerPosition.z);

    std::string playerPosText("Player Position: [" + std::to_string(info.playerPosition.x) + " - " + std::to_string(info.playerPosition.y) + " - " + std::to_string(info.playerPosition.z) + "]");
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
