#ifndef U_TRANSFORM
#define U_TRANSFORM

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Transform
{
public:
    Transform();

    // --- Position ---
    glm::vec3 GetPosition();
    void SetPosition(float x, float y, float z);
    void Translate(float dx, float dy, float dz);

    // --- Rotation ---
    glm::vec3 GetRotation();
    void SetRotation(float x, float y, float z);
    void Rotate(float dx, float dy, float dz);

    //Local rotation vectors
    glm::vec3 Forward();
    glm::vec3 Right();
    glm::vec3 Up();

    // --- Scaling ---
    glm::vec3 GetScale();
    void SetScale(float x, float y, float z);

private:
    //Single source of truth, don't use anything else pls
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif