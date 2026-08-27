#include "Game/world.hpp"

void World::Init()
{
    for (size_t i = 0; i < ROWS; i++)
    {
        for (size_t j = 0; j< COLS; j++)
        {
            shipGrid[i][j] = TileType::empty;
        } 
    }
}