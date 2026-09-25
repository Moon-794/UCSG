#include "Engine/Physics/physics.hpp"

namespace Physics
{
    bool Raycast(const glm::vec3& origin, const glm::vec3 direction, float distance, const glm::vec3& boxMin, const glm::vec3& boxMax, glm::vec3& hitPosition, glm::vec3& hitNormal)
    {
        glm::vec3 normDirection = glm::normalize(direction);

        float tMin = -std::numeric_limits<float>::infinity();
        float tMax =  std::numeric_limits<float>::infinity();
        glm::vec3 normal(0.0f);

        constexpr float epsilon = 1e-6f;
        
        //Slab Test
        for (size_t i = 0; i < 3; i++)
        {
            if (std::abs(normDirection[i]) < epsilon)
            {
                // Ray is parallel to this pair of planes.
                // It must already be inside the slab.
                if (origin[i] < boxMin[i] || origin[i] > boxMax[i])
                    return false;

                continue;
            }

            float inverseDirection = 1.0f / normDirection[i];

            float t1 = (boxMin[i] - origin[i]) * inverseDirection;
            float t2 = (boxMax[i] - origin[i]) * inverseDirection;

            glm::vec3 n1(0.0f);
            glm::vec3 n2(0.0f);

            n1[i] = -1.0f;
            n2[i] =  1.0f;

            if(t1 > t2)
            {
                std::swap(t1, t2);
                std::swap(n1, n2);
            }

            if (t1 > tMin)
            {
                tMin = t1;
                normal = n1;
            }
            
            tMax = std::min(tMax, t2);

            if (tMin > tMax || tMax < 0.0f)
                    return false;
        }

        if (tMin < 0.0f || tMin > distance)
            return false;
        
        hitPosition = origin + normDirection * tMin;
        hitNormal = normal;
        return true;
    }

    bool AABB_AABB_Collision(const AABB& a, const AABB& b)
    {
        return 
        (
            a.min.x <= b.max.x && a.max.x >= b.min.x &&
            a.min.y <= b.max.y && a.max.y >= b.min.y &&
            a.min.z <= b.max.z && a.max.z >= b.min.z
        );
    }

    void Resolve_AABB_AABB_Collision(const AABB& a, const AABB& b, Transform& playerTransform)
    {
        float overlapX = std::min(a.max.x, b.max.x) - std::max(a.min.x, b.min.x);
        float overlapY = std::min(a.max.y, b.max.y) - std::max(a.min.y, b.min.y);
        float overlapZ = std::min(a.max.z, b.max.z) - std::max(a.min.z, b.min.z);

        glm::vec3 centerA = (a.min + a.max) * 0.5f;
        glm::vec3 centerB = (b.min + b.max) * 0.5f;

        if(overlapX <= overlapY && overlapX <= overlapZ)
        {
            if(centerA.x < centerB.x)
            {
                playerTransform.Translate(-overlapX, 0.0f, 0.0f);
            }
            else
            {
                playerTransform.Translate(overlapX, 0.0f, 0.0f);
            }
        } 
        else if(overlapY <= overlapZ && overlapY <= overlapX)
        {
            if(centerA.y < centerB.y)
            {
                playerTransform.Translate(0.0f, -overlapY, 0.0f);
            }
            else
            {
                playerTransform.Translate(0.0f, overlapY, 0.0f);
            }
        }
        else
        {
            if(centerA.z < centerB.z)
            {
                playerTransform.Translate(0.0f, 0.0f, -overlapZ);
            }
            else
            {
                playerTransform.Translate(0.0f, 0.0f, overlapZ);
            }
        }
    }
}