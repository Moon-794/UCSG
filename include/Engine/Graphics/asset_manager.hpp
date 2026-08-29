#ifndef U_ASSET_MANAGER
#define U_ASSET_MANAGER

#include "Engine/Graphics/shader.hpp"
#include "Engine/Graphics/sprite.hpp"

#include "Third_Party/stb_image.h"

#include <unordered_map>
#include <regex>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// --------------- Asset manager is strictly renderer only ----------------------

//Asset manager holds references to commonly used data types
//such as texture IDS, shader programs, sounds etc. useful for render

struct RawTextureData
{
    std::string spriteName;
    unsigned int width, height, channels;
    std::vector<unsigned char*> imageData;
};

class AssetManager
{
public:
    AssetManager();

    void LoadShaders();
    void LoadTextures();

    unsigned int GetTexture(const std::string& textureName) const;
    std::shared_ptr<Shader> GetShader(const std::string& shaderName) const;
    
private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, unsigned int> textureMap;
};

#endif