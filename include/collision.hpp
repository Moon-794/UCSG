#ifndef U_COLLISION
#define U_COLLISION

#include <glm/glm.hpp>

const float SKIN = 0.001f;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
} URect;

bool PlayerAABBIntersectX(URect& player, URect& other);

bool PlayerAABBIntersectY(URect& player, URect& other);

bool PlayerAABBIntersect(URect& player, URect& other);

#endif