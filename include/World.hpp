#ifndef U_WORLD
#define U_WORLD

#include <cstdint>
#include <vector>

const int worldWidth = 256;
const int worldHeight = 256;

enum TileType
{
    Void,
    Floor
};

struct Tile
{
public:
    TileType type = TileType::Void;
};

struct World
{
public:
    World(int width, int height)
    {
        Tile emptyTile;
        emptyTile.type = TileType::Void;

        for (size_t i = 0; i < width * height; i++)
        {
            tiles.push_back(emptyTile);       
        }
    }

    Tile& GetTile(World& world, uint8_t x, uint8_t y)
    {
        return world.tiles[(worldWidth * y) + x];
    }

private:
    std::vector<Tile> tiles;
};

#endif