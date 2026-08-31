#include "Game/world.hpp"

void World::Init()
{
    for (size_t i = 0; i < 32; i++)
    {
        for (size_t j = 0; j < 32; j++)
        {
            if(i < 8 && j < 8)
                shipGrid[i][j] = TileType::ship;
            else
                shipGrid[i][j] = TileType::empty;
        } 
    }

    shipGrid[0][7] = TileType::empty;
    shipGrid[7][7] = TileType::empty;

    shipGrid[13][13] = TileType::ship;
}