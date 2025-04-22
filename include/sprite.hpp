#ifndef U_SPRITE
#define U_SPRITE

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shader.hpp"

namespace SpriteConstants
{
    const float vertices[] = 
    {
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    const unsigned int indices[] = 
    {
        0, 1, 3,
        1, 2, 3 
    };
}

unsigned int GenerateTextureID(std::string file)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load and generate the texture
    int width, height, nrChannels;
    std::string filePath = "resources/sprites/" + file;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, STBI_rgb_alpha);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
        return -1;
    }

    //Free stb data
    stbi_image_free(data);

    return texture;
}

class Sprite
{
public:
    Sprite(unsigned int textureID, glm::vec2 position, Shader* s)
    {
        this->textureID = textureID;
        this->position = position;
        scale = glm::vec2(1, 1);

        shader = s;
    }

    Sprite(std::string textureFile, glm::vec2 position, Shader* s)
    {
        textureID = GenerateTextureID(textureFile.c_str());
        this->position = position;
        scale = glm::vec2(1, 1);

        shader = s;
    }

    unsigned int textureID;
    Shader* shader;

    glm::vec2 position;
    glm::vec2 scale;
};

#endif