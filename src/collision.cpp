#include "collision.hpp"

bool AABBIntersectX(URect& a, URect& b)
{
    return (a.x < b.x + b.width - SKIN && a.x + a.width > b.x + SKIN);
}

bool AABBIntersectY(URect& a, URect& b)
{
    return (a.y < b.y + b.height - SKIN && a.y + a.height > b.y + SKIN);
}

bool AABBIntersect(URect& a, URect& b)
{
    return AABBIntersectX(a, b) && AABBIntersectY(a, b);
}

void ResolveSeperateAxisCollision(URect& a, URect& b)
{
    
}