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

    constexpr static int ROWS = 16;
    constexpr static int COLS = 16;

    std::array<std::array<TileType, COLS>, ROWS> shipGrid {};
};

#endif