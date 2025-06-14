#include "debugger.hpp"

Debugger::Debugger(Renderer* renderer)
{
    this->renderer = renderer;

    debugShader = std::make_shared<Shader>(Shader("resources/shaders/debugger/vertex.vert", "resources/shaders/debugger/fragment.frag"));
    debugSprite = Sprite("d_selector.png", glm::vec2(0, 0), debugShader);
    debugSprite.scale = glm::vec2(1, 1);

    //Enable ImGUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(renderer->window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void Debugger::DrawDebugger(DebuggerInfo info)
{
    ProcessDebugger(info);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Debugger");
    int roundedX = std::round(info.playerPosition.x * 100.0f);
    int roundedY = std::round(info.playerPosition.y * 100.0f);

    std::string playerPosText("Player Position: [" + std::to_string(roundedX) + " - " + std::to_string(roundedY) + "]");
    ImGui::Text(playerPosText.c_str());

    ImGui::Checkbox("Show Active Colliders", &state.showActiveColliders);

    std::string areaDataText("Area: " + info.areaData.areaName);
    ImGui::Text(areaDataText.c_str());  

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Debugger::ProcessDebugger(DebuggerInfo info)
{
    if(state.showActiveColliders)
    {
        float playerX = glm::floor(info.playerPosition.x + 0.5f);
        float playerY = glm::floor(info.playerPosition.y + 0.5f);

        const glm::vec3 redColor(1, 0, 0);
        const glm::vec3 blueColor(0, 0, 1);

        for (int x = -1; x < 2; x++)
        {
            for (int y = -1; y < 2; y++)
            {   
                debugSprite.position = glm::vec3(playerX + x, playerY + y, 0);

                debugShader->use();
                debugShader->setVec3("tcolor", redColor);

                if(debugSprite.position.x >= 0 && debugSprite.position.y >= 0)
                {
                    if(debugSprite.position.x < info.areaData.areaWidth && debugSprite.position.y < info.areaData.areaHeight)
                    {
                            int tileID = info.areaData.tileLayers[0].layerData[debugSprite.position.x][debugSprite.position.y];
                            bool isCollider = info.areaData.tileLayers[0].collisionMap.at(tileID);
                            
                            if(isCollider == true)
                            {
                                 debugShader->setVec3("tcolor", blueColor);
                            }
                            else
                            {
                                debugShader->setVec3("tcolor", redColor);
                            }

                            DrawSprite((*renderer), debugSprite);
                    }
                }

            }
        }
    }
}
