#ifndef U_PHYSICS
#define U_PHYSICS

#include "Engine/transform.hpp"
#include "stdbool.h"

namespace Physics
{
    //Returns if a ray hits a given AABB, and stores the hit position in ``glm::vec3& hitPosition``
    bool Raycast(const glm::vec3& origin, const glm::vec3 direction, const glm::vec3& boxTransform, glm::vec3& hitPosition);

    //AABB vs AABB Collision Check
    bool AABB_AABB_Collision(const glm::vec3& minA, const glm::vec3& maxA, const glm::vec3& minB, const glm::vec3& maxB);

    //Resolve an AABB_AABB collision test thats come back positive
    void Resolve_AABB_AABB_Collision(glm::vec3 minA, glm::vec3 maxA, glm::vec3 minB, glm::vec3 maxB, Transform& playerTransform, glm::vec3& velocity);
}

#endif