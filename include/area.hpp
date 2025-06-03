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

            std::cout << json_object_get_string(layerType) << "\n";
            
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

#endif