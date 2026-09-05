#ifndef U_SPRITE
#define U_SPRITE

#include "Third_Party/stb_image.h"
#include "Engine/Graphics/shader.hpp"

#include <vector>

namespace MeshConstants
{
    const float vertices[] = 
    {
        1.0f,  0.0f, 0.0f, 1.0f, 1.0f, // Top Right
        1.0f,  -1.0f, 0.0f, 1.0f, 0.0f, // Bottom Right
        0.0f,  -1.0f, 0.0f, 0.0f, 0.0f, // Bottom Left
        0.0f,  0.0f, 0.0f, 0.0f, 1.0f  // Top Left
    };

    const unsigned int indices[] = 
    {
        0, 1, 3,
        1, 2, 3 
    };

    const float cubeVertices[] =
    {
        // Front (+Z)
        0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        1.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        0.0f, 1.0f, 1.0f,   0.0f, 1.0f,

        // Back (-Z)
        1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        0.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        1.0f, 1.0f, 0.0f,   0.0f, 1.0f,

        // Left (-X)
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
        0.0f, 1.0f, 1.0f,   1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

        // Right (+X)
        1.0f, 0.0f, 1.0f,   0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,   0.0f, 1.0f,

        // Top (+Y)
        0.0f, 1.0f, 1.0f,   0.0f, 0.0f,
        1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
        1.0f, 1.0f, 0.0f,   1.0f, 1.0f,
        0.0f, 1.0f, 0.0f,   0.0f, 1.0f,

        // Bottom (-Y)
        0.0f, 0.0f, 0.0f,   0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
        1.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        0.0f, 0.0f, 1.0f,   0.0f, 1.0f
    };

    const int cubeIndices[] =
    {
        // Front
        0,  1,  2,
        2,  3,  0,

        // Back
        4,  5,  6,
        6,  7,  4,

        // Left
        8,  9, 10,
        10, 11,  8,

        // Right
        12, 13, 14,
        14, 15, 12,

        // Top
        16, 17, 18,
        18, 19, 16,

        // Bottom
        20, 21, 22,
        22, 23, 20
    };
}

unsigned int GenerateTextureID(std::string file);

class Sprite
{
public:

    Sprite()
    {
        
    }

    Sprite(unsigned int textureID, glm::vec2 position, std::shared_ptr<Shader> s)
    {
        this->textureID = textureID;
        this->position = position;
        scale = glm::vec2(1, 1);

        shader = s;
    }

    Sprite(std::string textureFile, glm::vec2 position, std::shared_ptr<Shader> s)
    {
        textureID = GenerateTextureID(textureFile.c_str());
        this->position = position;
        scale = glm::vec2(1, 1);

        shader = s;
    }

    unsigned int textureID;
    std::shared_ptr<Shader> shader;

    glm::vec2 position;
    glm::vec2 scale;
};

#endif