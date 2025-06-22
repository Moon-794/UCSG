#include "asset_manager.hpp"

AssetManager::AssetManager()
{
    std::cout << "Loading resources..." << std::endl;

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
    //Get all shader folders in resources/shaders
    int folderCount = 0;

    const std::string resourcesPath = "resources/textures";
    for(const auto& entry : fs::directory_iterator(resourcesPath))
    {
        //Gives a linux + windows friendly folderpath
        std::string folderPath = std::regex_replace(entry.path().string(), std::regex("\\\\"), "/");

        //Erase the first part of the path to use in file names
        std::string textureName(entry.path().filename().string());

        unsigned int textureID = GenerateTextureID(textureName);
        textureMap.insert({textureName, textureID});

        folderCount++;
    }

    std::cout << "Loaded " << folderCount << " textures..." << std::endl;
}

unsigned int AssetManager::GetTexture(const std::string& textureName) const 
{
    return 0;
}

std::shared_ptr<Shader> AssetManager::GetShader(const std::string& shaderName) const
{
    return shaderMap.at(shaderName);
}