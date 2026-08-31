#ifndef U_CAMERA
#define U_CAMERA

#include "Engine/transform.hpp"
#include "stdbool.h"

// What should camera do?
// > Store it's transform
// > Store Projection information and matrix

class Camera
{
public:
    Transform transform;

    void SetFOV(float FOV);
    float GetFOV() const;

    void SetAspectRatio(float aspectRatio);
    float GetAspectRatio() const;

    const glm::mat4& GetProjection() const;
    glm::mat4 GetViewMatrix();

private:
    float fov = 80.0f;
    float aspectRatio = 1.0f;

    void CalculateProjection();

    glm::mat4 projection;
};

#endif