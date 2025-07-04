#ifndef U_SPRITE
#define U_SPRITE

#include "stb_image.h"
#include "shader.hpp"

namespace SpriteConstants
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