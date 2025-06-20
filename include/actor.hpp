#ifndef U_PLAYER
#define U_PLAYER

#include "game.hpp"

class Actor
{
public:
    Actor(glm::vec2 position, glm::vec2 scale);

    void SetPosition(float x, float y);
    void Move(float dx, float dy);
    
private:
    glm::vec2 position;
    glm::vec2 scale;

    void MoveX(float amount);
    void MoveY(float amount);
};

#endif