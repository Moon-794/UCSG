#ifndef U_GAMESTATE
#define U_GAMESTATE

#include "area.hpp"


//Gamestate holds all relevant data for the game, including player information, area information, 
//current high level state (playing, paused, title screen) etc
struct GameState
{
public:
    Area* current
};

#endif