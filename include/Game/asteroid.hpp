#ifndef U_ASTEROID
#define U_ASTEROID

#include <string>

class Asteroid
{
    Transform transform;

    std::string materialType;
    int remainingMaterial = 100;

    float radius;
};

#endif