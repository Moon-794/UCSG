#ifndef U_WORLD
#define U_WORLD

#include "stdio.h"

struct World
{
    constexpr int ROWS = 16;
    constexpr int COLS = 16;

    std::array<std::array<bool, ROWS>, COLS> shipGrid {};
};

#endif