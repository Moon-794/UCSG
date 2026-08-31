#include "Game/world.hpp"

void World::Init()
{
    for (size_t i = 0; i < 32; i++)
    {
        for (size_t j = 0; j < 32; j++)
        {
            if(i < 7 && j < 12)
                shipGrid[i][j] = TileType::ship;
            else
                shipGrid[i][j] = TileType::empty;
        } 
    }

    shipGrid[0][11] = TileType::empty;
    shipGrid[6][11] = TileType::empty;

    shipGrid[4][0] = TileType::door;
}