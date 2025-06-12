#ifndef U_AREA
#define U_AREA

#include "renderer.hpp"

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include <regex>

#include <json-c/json.h>

namespace fs = std::filesystem;

struct Layer
{
public:
    std::vector<std::vector<int>> tiles;
    unsigned int textureID;
};

//JSON RELATED

int JSONGetInt(json_object* obj, std::string fieldName);
json_object* GetRootAreaDataFromFile(std::string areaName);

// __ __ __ __ _ _ _ _

//IMAGE RELATED

struct ImageData
{
    int width;
    int height;
    int nrChannels;

    unsigned char* data;
};

unsigned int LoadImageData(const std::string& areaName, ImageData& data);

// -- --------   -----

std::map<int, bool> GenerateCollisionMap(std::string areaData);

struct TileLayer
{
    std::vector<std::vector<int>> layerData;
    Sprite layerSprite;
};

struct Tile
{
    std::string tileType;
    std::unordered_map<std::string, std::string> tileProperties;
};

struct AreaTransition
{
    int newAreaID;
    glm::vec2 spawnPosition;
};

struct AreaData
{
    unsigned int areaID;
    std::string areaName;                                       //Used for area transitions

    unsigned int areaWidth, areaHeight;                         //Width and height map in tiles
    unsigned int tileWidth, tileHeight;                         //Width and height of tiles in pixels

    std::unordered_map<unsigned int, Tile> tileset;             //holds tileIDS -> tileProperties
    std::vector<TileLayer> tileLayers;                          //Map + collision data
    
    std::unordered_map<std::string, std::string> properties;
};

//This is looking to be a core game system, on par with the renderer
//Although I'm not too sure how I want different game systems similar to this one to easily talk to each other
class AreaManager
{
public:
    AreaManager(std::shared_ptr<Shader> areaShader);

    void DrawCurrentAreaLayer(int layerIndex);

    std::shared_ptr<AreaData> GetCurrentArea();
    std::shared_ptr<AreaData> getArea(std::string areaName);
    std::shared_ptr<AreaData> getArea(unsigned int areaID);

    void Transition(AreaTransition areaTransition);
    void ForceTransition(std::string areaName, glm::vec2 spawnPosition);

private:
    std::unordered_map<std::string, std::shared_ptr<AreaData>> areas;

    std::shared_ptr<AreaData> currentArea;
    unsigned int nextAvailableID = 0;

    std::shared_ptr<Shader> areaShader;

    std::unordered_map<std::string, std::shared_ptr<AreaData>> LoadAllAreas();

    AreaData LoadAreaData(std::string areaName);
    std::unordered_map<std::string, std::string> LoadAreaProperties(std::string areaName);
    std::unordered_map<unsigned int, Tile> LoadTileset(std::string areaName);
    std::vector<TileLayer> LoadLayers(std::string areaName);

    unsigned int LoadTileLayer(TileLayer& tileLayer, const json_object* element);
    unsigned int GenerateLayerTextureID(const std::string& areaName, const std::vector<std::vector<int>>& tileIDS, int spriteWidth, int mapWidth, int mapHeight);

    void AddArea(AreaData* areaData);
    void RemoveArea(unsigned int areaID);
    void RemoveArea(std::string areaName);
};

#endif