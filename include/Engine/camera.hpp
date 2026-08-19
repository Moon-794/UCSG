#ifndef U_CAMERA
#define U_CAMERA

#include "Engine/transform.hpp"

// What should camera do?
// > Store it's transform
// > Store Projection information and matrix

class Camera
{
public:
    Transform transform;
};

#endif