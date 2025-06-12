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

unsigned int LoadImageData(const std::string& areaName, ImageData& data);

// -- --------   -----

Layer ProcessAreaLayer(json_object* layerData);
std::map<int, bool> GenerateCollisionMap(std::string areaData);

struct Area
{
public:
    Area(std::string areaData)
    {
        ProcessTilemaps(areaData);

        layerSprite = Sprite(-1, glm::vec2(0, 0), nullptr);
        layerSprite.position = glm::vec2(0, 0);
        layerSprite.scale = glm::vec2(32, 32);

        collisionMap = GenerateCollisionMap(areaData);
    }

    void ProcessTilemaps(std::string areaData)
    {
        std::string filePath("resources/areaData/" + areaData + "/data.json");
        json_object *root = json_object_from_file(filePath.c_str());

        json_object *layers;
        json_object_object_get_ex(root, "layers", &layers);
        int array_len = json_object_array_length(layers);

        for (size_t i = 0; i < array_len; i++)
        {
            json_object *elem = json_object_array_get_idx(layers, i);

            json_object* layerType;
            json_object_object_get_ex(elem, "type", &layerType);
         
            if(std::string(json_object_get_string(layerType)) == "tilelayer")
            {
                json_object *data;
                json_object_object_get_ex(elem, "data", &data);
                
                this->layers.push_back(ProcessAreaLayer(data));
            }
        }
    }

    void DrawLayer(Renderer& renderer, int index)
    {
        layerSprite.textureID = layers[index].textureID;
        DrawSprite(renderer, layerSprite);
    }

    void SetShader(Shader* s)
    {
        layerSprite.shader = s;
    }

    int& GetTile(uint8_t x, uint8_t y)
    {
        return layers[0].tiles[x][y];
    }

    std::vector<Layer> layers;
    std::map<int, bool> collisionMap;

private:
    Sprite layerSprite;
    
    unsigned int areaWidth;
    unsigned int areaHeight;
};

struct ImageData
{
    int width;
    int height;
    int nrChannels;

    unsigned char* imageData;
};

struct TileLayer
{
    std::vector<std::vector<int>> layerData;
    unsigned int layerTextureID;
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
    AreaManager();

    AreaData* getCurrentArea();
    AreaData* getArea(std::string areaName);
    AreaData* getArea(unsigned int areaID);

    void Transition(AreaTransition areaTransition);
    void ForceTransition(std::string areaName, glm::vec2 spawnPosition);

private:
    std::unordered_map<std::string, std::shared_ptr<AreaData>> areas;

    AreaData* currentArea;
    unsigned int nextAvailableID = 0;

    std::unordered_map<std::string, std::shared_ptr<AreaData>> LoadAllAreas();

    AreaData LoadAreaData(std::string areaName);
    std::unordered_map<std::string, std::string> LoadAreaProperties(std::string areaName);
    std::unordered_map<unsigned int, Tile> LoadTileset(std::string areaName);
    std::vector<TileLayer> LoadLayers(std::string areaName);

    unsigned int LoadTileLayer(TileLayer& tileLayer, const json_object* element);
    unsigned int GenerateLayerTextureID(const std::string& areaName, const std::vector<std::vector<int>>& layerData, int spriteWidth);

    void AddArea(AreaData* areaData);
    void RemoveArea(unsigned int areaID);
    void RemoveArea(std::string areaName);
};

#endif