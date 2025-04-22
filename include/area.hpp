#ifndef U_AREA
#define U_AREA

#include <cstdint>
#include <vector>
#include <json-c/json.h>

enum TileType
{
    Void,
    Floor
};

struct Tile
{
public:
    bool collider = false;
};

struct Area
{
public:
    Area(std::string areaData)
    {
        
    }

    Tile& GetTile(uint8_t x, uint8_t y)
    {
        return tiles[(areaWidth * y) + x];
    }

private:
    std::vector<Tile> tiles;
    
    int areaWidth;
    int areaHeight;
};

#endif