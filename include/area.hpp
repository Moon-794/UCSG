#ifndef U_AREA
#define U_AREA

#include "renderer.hpp"

#include <map>
#include <string>
#include <cstdint>
#include <vector>
#include <json-c/json.h>

struct Layer
{
public:
    std::vector<std::vector<int>> tiles;
    unsigned int textureID;
};

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
        std::string filePath("resources/areaData/" + areaData + "/" + areaData + ".json");
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
                std::cout << "hha" << std::endl;
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

//Additional stuff:
//Give areas a custom property of vector2: "DefaultSpawn" for warping
//Or even a list of spawn locations for use in the debugger

enum LayerType
{
    TILE_LAYER,
    COLLISION_LAYER,
    OBJECT_GROUP
};

struct TileLayer
{
    std::vector<int> tileData;
    unsigned int layerTextureID;
};

//AreaObjects might be silly, could perhaps be better to have a general object instead?
//For the sake of not getting into the DynamicObject + staticObject problem, might want a mix
struct AreaObject
{

}

struct ObjectGroup
{
    //List of specific types of objects
    //Concrete objects such as area transitions will have a class associated with them
}

struct AreaTransition
{
    int newAreaID;
    glm::vec2 spawnPosition;
};

struct Tileset
{
    //A map of tileIDS to tile properties
};

struct AreaData
{
    unsigned int areaID;
    std::string areaName;                    //Used for area transitions

    unsigned int areaWidth, areaHeight;     //Width and height map in tiles
    unsigned int tileWidth, tileHeight;     //Width and height of tiles in pixels

    Tileset areaTileset;
    std::vector<TileLayer> tileLayers;      //Map + collision data  
};

//Liking this so far
//This is looking to be a core game system, on par with the renderer
class AreaManager
{
    AreaData* getCurrentArea();
    AreaData* getArea(std::string areaName);

    void Transition(AreaTransition areaTransition);

private:
    std::unordered_map<std::string, AreaData*> areas;
    AreaData* currentArea;
}

#endif