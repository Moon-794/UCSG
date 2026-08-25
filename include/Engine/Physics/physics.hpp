#ifndef U_PHYSICS
#define U_PHYSICS

#include "Engine/transform.hpp"
#include "stdbool.h"

namespace Physics
{
    //Returns if a ray hits a given AABB, and stores the hit position in ``glm::vec3& hitPosition``
    bool Raycast(const glm::vec3& origin, const glm::vec3 direction, const glm::vec3& boxTransform, glm::vec3& hitPosition);
}

#endif