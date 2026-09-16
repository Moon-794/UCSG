#ifndef U_PHYSICS
#define U_PHYSICS

#include "Engine/transform.hpp"
#include "Engine/Physics/AABB.hpp"
#include "stdbool.h"

namespace Physics
{
    //Returns if a ray hits a given AABB, and stores the hit position in ``glm::vec3& hitPosition``
    bool Raycast(const glm::vec3& origin, const glm::vec3 direction, float distance, const AABB& aabb, glm::vec3& hitPosition, glm::vec3& hitNormal);

    //AABB vs AABB Collision Check
    bool AABB_AABB_Collision(const AABB& a, const AABB& b);

    //Resolve an AABB_AABB collision test thats come back positive
    void Resolve_AABB_AABB_Collision(const AABB& a, const AABB& b, Transform& playerTransform);
}

#endif