#include "Engine/Graphics/asset_manager.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Third_Party/stb_image.h"

AssetManager::AssetManager()
{
    std::cout << "Loading resources..." << std::endl;

    //Y flipping on images
    stbi_set_flip_vertically_on_load(true);

    LoadShaders();
    LoadTextures();
}

//Create and store a shader for each subfolder in the resources/shaders directory
void AssetManager::LoadShaders()
{
    //Get all shader folders in resources/shaders
    int folderCount = 0;

    const std::string resourcesPath = "resources/shaders";
    for(const auto& entry : fs::directory_iterator(resourcesPath))
    {
        //Gives a linux + windows friendly folderpath
        std::string folderPath = std::regex_replace(entry.path().string(), std::regex("\\\\"), "/");

        //Erase the first part of the path to use in file names
        std::string shaderName(entry.path().filename().string());

        std::string vert = folderPath + "/vertex.vert";
        std::string frag = folderPath + "/fragment.frag";
        std::shared_ptr<Shader> shader = std::make_shared<Shader>(shaderName.c_str(), vert.c_str(), frag.c_str());
        
        shaderMap.insert({shaderName, shader});

        folderCount++;
    }

    std::cout << "Loaded " << folderCount << " shaders..." << std::endl;
}

void AssetManager::LoadTextures()
{
    int folderCount = 0;

    const std::string resourcesPath = "resources/textures";
    for(const auto& entry : fs::directory_iterator(resourcesPath))
    {
        std::string folderPath = std::regex_replace(entry.path().string(), std::regex("\\\\"), "/");

        int width, height, nrChannels;
        unsigned char *data = stbi_load(folderPath.c_str(), &width, &height, &nrChannels, 0);

        if (!data) 
        {
            std::cerr << "Failed to load: " << entry.path().string()
                    << "\n" << stbi_failure_reason() << '\n';
            return;
        }
        
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        
        stbi_image_free(data);

        std::string textureName(entry.path().stem().string());
        textureMap.insert({textureName, textureID});
    }
}

std::shared_ptr<Shader> AssetManager::GetShader(const std::string& shaderName) const
{
    return shaderMap.at(shaderName);
}

unsigned int AssetManager::GetTexture(const std::string& textureName) const
{
    return textureMap.at(textureName);
}