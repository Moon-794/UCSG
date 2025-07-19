#include "area.hpp"

AreaManager::AreaManager()
{
    std::cout << "Initialising Area Manager" << "\n";

    //areas = AreaManager::LoadAllAreas();
    currentArea = areas["Hallway"];
}

void Transition(AreaTransition areaTransition)
{

}

void ForceTransition(std::string areaName, glm::vec2 spawnPosition, float& playerX, float& playerY)
{

}