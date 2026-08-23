#ifndef U_ASTEROID
#define U_ASTEROID

#include <string>
#include <stdbool.h>

class Asteroid
{
public:
    Transform transform;

    std::string materialType;
    int remainingMaterial = 100;

    bool isActive = true;
};

#endif