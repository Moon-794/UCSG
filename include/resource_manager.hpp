#ifndef U_RESOURCE_MANAGER
#define U_RESOURCE_MANAGER

#include "shader.hpp"
#include "sprite.hpp"

#include <unordered_map>
#include <regex>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

struct FolderData
{

};

//Resource manager holds references to commonly used data types
//such as texture IDS, shader programs, sounds, tilesets etc.
class ResourceManager
{
public:
    ResourceManager();

    void LoadShaders();
    void LoadTextures();

    unsigned int GetTexture(const std::string& textureName) const;
    std::shared_ptr<Shader> GetShader(const std::string& shaderName) const;

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, unsigned int> textureMap;
};

#endif