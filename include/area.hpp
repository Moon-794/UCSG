#ifndef U_AREA
#define U_AREA

#include <cstdint>
#include <vector>
#include <json-c/json.h>

std::vector<bool> GenerateCollisionMap();

struct Tile
{

};

struct Layer
{
    std::vector<Tile> tiles;
    unsigned int textureID;
};

struct Area
{
public:
    Area(std::string areaData)
    {
        
    }

    Tile& GetTile(uint8_t x, uint8_t y)
    {
        return layers[0].tiles[(areaWidth * y) + x];
    }

private:
    std::vector<Layer> layers;
    std::vector<bool> collisionMap;
    
    unsigned int areaWidth;
    unsigned int areaHeight;
};

std::vector<bool> GenerateCollisionMap()
{
    return {false};
}

#endif