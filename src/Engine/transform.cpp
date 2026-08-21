#include "Engine/transform.hpp"

Transform::Transform()
{
    //Do nothing
}

glm::vec3 Transform::GetRotation()
{
    return rotation;
}

void Transform::SetRotation(float x, float y, float z)
{
    rotation = glm::vec3(rotation.x, rotation.y, rotation.z);
}

void Transform::Rotate(float dx, float dy, float dz)
{
    rotation += glm::vec3(dx, dy, dz);
}

glm::vec3 Transform::Forward()
{
    glm::vec3 forward = glm::vec3(0.0f, 0.0f, 0.0f);

    forward.x = glm::cos(glm::radians(rotation.x)) * glm::sin(glm::radians(rotation.y));
    forward.y = glm::sin(glm::radians(rotation.x));
    forward.z = glm::cos(glm::radians(rotation.x)) * glm::cos(glm::radians(rotation.y));

    return glm::normalize(forward);
}

glm::vec3 Transform::Right()
{
    glm::vec3 right = glm::vec3(0.0f, 0.0f, 0.0f);

    float p = glm::radians(rotation.x);
    float y = glm::radians(rotation.y);
    float r = glm::radians(rotation.z);

    right.x = glm::cos(y) * glm::cos(r) - glm::sin(p) * glm::sin(y) * glm::sin(r);
    right.y = glm::cos(p) * glm::sin(r);
    right.z = -glm::sin(y) * glm::cos(r) - glm::sin(p) * glm::cos(y) * glm::sin(r);

    return glm::normalize(right);
}

glm::vec3 Transform::Up()
{
    glm::vec3 up = glm::vec3(0.0f, 0.0f, 0.0f);

    float p = glm::radians(rotation.x);
    float y = glm::radians(rotation.y);
    float r = glm::radians(rotation.z);

    up.x = -glm::sin(p) * glm::sin(y) * glm::cos(r) - glm::cos(y) * glm::sin(r);
    up.y = glm::cos(p) * glm::cos(r);
    up.z = -glm::sin(p) * glm::cos(y) * glm::cos(r) + glm::sin(y) * glm::sin(r);

    return glm::normalize(up);
}

glm::vec3 Transform::GetPosition()
{
    return position;
}

void Transform::SetPosition(float x, float y, float z)
{
    position = glm::vec3(x, y, z);
}

void Transform::Translate(float dx, float dy, float dz)
{
    position += glm::vec3(dx, dy, dz);
}

void Transform::Translate(glm::vec3 translationVector)
{
    position += translationVector;
}

glm::vec3 Transform::GetScale()
{
    return scale;
}

void Transform::SetScale(float x, float y, float z)
{
    scale = glm::vec3(x, y, z);
}