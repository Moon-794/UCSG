#ifndef U_ASSET_MANAGER
#define U_ASSET_MANAGER

#include "shader.hpp"
#include "sprite.hpp"

#include <unordered_map>
#include <regex>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class AreaTextures
{
    std::vector<unsigned int> sprites;
    bool isProcedural = false;
};

//Resource manager holds references to commonly used data types
//such as texture IDS, shader programs, sounds etc.
class AssetManager
{
public:
    AssetManager();

    void LoadShaders();
    void LoadTextures();

    unsigned int GetTexture(const std::string& textureName) const;
    std::shared_ptr<Shader> GetShader(const std::string& shaderName) const;

    AreaTextures GetAreaTextures(const AreaData& areaData) const;
    AreaTextures GenerateAreaTextures(const AreaData& areaData) const;
    unsigned int GenerateLayerTexture(const AreaData& areaData, int layer) const;
    
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, unsigned int> textureMap;

    std::unordered_map<std::string, AreaTextures> areaTextures;
};

#endif