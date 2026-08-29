#ifndef U_WORLD
#define U_WORLD

#include "stdio.h"
#include <array>

enum TileType
{
    empty,
    ship,
    door
};

struct World
{
public:
    void Init();

    constexpr static int ROWS = 32;
    constexpr static int COLS = 32;

    std::array<std::array<TileType, ROWS>, COLS> shipGrid {};
};

#endif