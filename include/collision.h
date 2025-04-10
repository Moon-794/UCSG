#ifndef U_COLLISION
#define U_COLLISION

#include <glm/glm.hpp>

typedef struct
{
    float x;
    float y;
    float width;
    float height;
} URect;

bool PlayerAABBIntersectX(URect& player, URect& other)
{
    return (player.x < other.x + other.width && player.x + player.width > other.x);
}

bool PlayerAABBIntersectY(URect& player, URect& other)
{
    return (player.y < other.y + other.height && player.y + player.height > other.y);
}

bool PlayerAABBIntersect(URect& player, URect& other)
{
    return PlayerAABBIntersectX(player, other) && PlayerAABBIntersectY(player, other);
}

#endif