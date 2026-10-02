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

    ImGui::Begin("Debugger", nullptr, ImGuiWindowFlags_MenuBar);

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("General"))
        {
            if (ImGui::MenuItem("Player"))  { /*Nothing so far*/ }
            if (ImGui::MenuItem("World"))   { /*Nothing so far*/ }
            if (ImGui::MenuItem("Physics")) { /*Nothing so far*/ }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::SeparatorText("Player Position");
    glm::vec3 pos = info.playerTransform->GetPosition();
    float debugPos[3] = {pos.x, pos.y, pos.z};
    ImGui::DragFloat3("", debugPos, 0.1f);

    if(debugPos[0] != pos.x || debugPos[1] != pos.y || debugPos[2] != pos.z)
    {
        info.playerTransform->SetPosition(debugPos[0], debugPos[1], debugPos[2]);
    }

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
