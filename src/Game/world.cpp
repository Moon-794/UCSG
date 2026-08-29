#include "Game/world.hpp"

void World::Init()
{
    for (size_t i = 0; i < 32; i++)
    {
        for (size_t j = 0; j < 32; j++)
        {
            shipGrid[i][j] = TileType::ship;
        } 
    }
}