#ifndef U_AREA
#define U_AREA

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <regex>
#include <iostream>

#include <json-c/json.h>
#include "glm/glm.hpp"

#include "tile_manager.hpp"

namespace fs = std::filesystem;

struct AreaTransition
{
    int newAreaID;
    glm::vec2 spawnPosition;
};

struct AnimatedTile
{
    const unsigned int x, y;                    //Position in tiles grid
    const std::vector<unsigned int> frames;      //What tile ID to return to if the animation is cancelled or restarted.
    const unsigned int ticksPerFrame;

    unsigned int ticks = 0;
};

struct Area
{
    const unsigned int id;
    const std::string name;                                           //Used for area transitions

    const unsigned int width, height;                                 //Width and height map in tiles
    const unsigned int tileSize;                                      //Width and height of tiles in pixels

    std::vector<std::vector<unsigned int>> tiles;
    std::vector<AnimatedTile> animatedTiles;

    std::unordered_map<std::string, std::string> properties;    //Custom properties of the area (e.g Type = "Ship", Room = "Quarters", Oxygen = "false")
};

class AreaManager
{
public:
    AreaManager();

    //const Area& getCurrentArea() const;

    void Transition(AreaTransition areaTransition);
    void ForceTransition(std::string areaName, glm::vec2 spawnPosition, float& playerX, float& playerY);

private:
    std::shared_ptr<Area> currentArea;
    std::unordered_map<std::string, std::shared_ptr<Area>> areas;

    //std::unordered_map<std::string, std::shared_ptr<Area>> LoadAllAreas();
};

#endif