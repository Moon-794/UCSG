#include "actor.hpp"

Actor::Actor(glm::vec2 position, glm::vec2 scale)
{
    this->position = position;
    this->scale = scale;
}

void Actor::Move(float dx, float dy)
{
    MoveX(dx);

    MoveY(dx);
}