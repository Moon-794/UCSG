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

bool AABBIntersectX(URect& player, URect& other);

bool AABBIntersectY(URect& player, URect& other);

bool AABBIntersect(URect& player, URect& other);

void ResolveSeperateAxisCollision(URect& a, URect& b);

#endif