#include "Engine/Physics/physics.hpp"

namespace Physics
{
    bool Raycast(const glm::vec3& origin, const glm::vec3 direction, const glm::vec3& boxPosition, glm::vec3& hitPosition)
    {
        glm::vec3 boxMin = boxPosition - glm::vec3(0.5f, 0.5f, 0.5f);
        glm::vec3 boxMax = boxPosition + glm::vec3(0.5f, 0.5f, 0.5f);

        float tMin = -std::numeric_limits<float>::infinity();
        float tMax =  std::numeric_limits<float>::infinity();

        constexpr float epsilon = 1e-6f;
        
        //Slab Test
        for (size_t i = 0; i < 3; i++)
        {
            if (std::abs(direction[i]) < epsilon)
            {
                // Ray is parallel to this pair of planes.
                // It must already be inside the slab.
                if (origin[i] < boxMin[i] || origin[i] > boxMax[i])
                    return false;

                continue;
            }

            float inverseDirection = 1.0f / direction[i];

            float t1 = (boxMin[i] - origin[i]) * inverseDirection;
            float t2 = (boxMax[i] - origin[i]) * inverseDirection;

            if(t1 > t2)
                std::swap(t1, t2);
            
            tMin = std::max(tMin, t1);
            tMax = std::min(tMax, t2);

            if (tMin > tMax || tMax < 0.0f)
                    return false;
        }
        
        hitPosition = origin + direction * tMin;
        return true;
    }
}