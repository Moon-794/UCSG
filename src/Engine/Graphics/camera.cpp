#include "Engine/Graphics/camera.hpp"

void Camera::SetFOV(const float FOV)
{
    this->fov = FOV;
    CalculateProjection();
}

float Camera::GetFOV() const
{
    return this->fov;
}

void Camera::SetAspectRatio(float aspectRatio)
{
    this->aspectRatio = aspectRatio;
    CalculateProjection();
}

float Camera::GetAspectRatio() const
{
    return aspectRatio;
}

void Camera::CalculateProjection()
{
    projection = glm::perspectiveLH(glm::radians(fov), aspectRatio, 0.0001f, 20000.0f);
}

const glm::mat4& Camera::GetProjection() const
{
    return projection;
}