#ifndef U_PHYSICS
#define U_PHYSICS

#include "Engine/transform.hpp"
#include "Engine/Physics/AABB.hpp"
#include "stdbool.h"

namespace Physics
{
    struct CollisionData
    {
        AABB plane;
        glm::vec3 hitPos;
        glm::vec3 normal;
        float time;
    };
    
    //Returns if a ray hits a given AABB, and stores the hit position in ``glm::vec3& hitPosition``
    bool Raycast(const glm::vec3& origin, const glm::vec3 direction, float distance, const glm::vec3& boxMin, const glm::vec3& boxMax, glm::vec3& hitPosition, glm::vec3& hitNormal, float& time);

    //AABB vs AABB Collision Check
    bool AABB_AABB_Collision(const AABB& a, const AABB& b);

    //Resolve an AABB_AABB collision test thats come back positive
    void Resolve_AABB_AABB_Collision(const AABB& a, const AABB& b, Transform& playerTransform);
}

#endif