#ifndef U_COLLISION
#define U_COLLISION

#include <glm/glm.hpp>

const float SKIN = 0.0001f;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
} URect;

bool PlayerAABBIntersectX(URect& player, URect& other)
{
    return (player.x < other.x + other.width - SKIN && player.x + player.width > other.x + SKIN);
}

bool PlayerAABBIntersectY(URect& player, URect& other)
{
    return (player.y < other.y + other.height - SKIN && player.y + player.height > other.y + SKIN);
}

bool PlayerAABBIntersect(URect& player, URect& other)
{
    return PlayerAABBIntersectX(player, other) && PlayerAABBIntersectY(player, other);
}

#endif