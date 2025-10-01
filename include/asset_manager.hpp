#ifndef U_ASSET_MANAGER
#define U_ASSET_MANAGER

#include "shader.hpp"
#include "sprite.hpp"

#include <unordered_map>
#include <regex>
#include <filesystem>
#include <string>

namespace fs = std::filesystem;

// --------------- Asset manager is strictly renderer only ----------------------

//Asset manager holds references to commonly used data types
//such as texture IDS, shader programs, sounds etc. useful for render

struct RawSpriteData
{
    std::string spriteName;
    unsigned int width, height, channels;
    std::vector<unsigned char*> imageData;
}

class AssetManager
{
public:
    AssetManager();

    void LoadShaders();
    void LoadSprites();

    unsigned int GetSpriteIndex(const std::string& spriteName) const;
    std::shared_ptr<Shader> GetShader(const std::string& shaderName) const;

private:
    void GenerateSpriteSheet(std::vector<RawSpriteData> sprites);


    std::unordered_map<std::string, std::shared_ptr<Shader>> shaderMap;
    std::unordered_map<std::string, unsigned int> textureMap;

    unsigned int spritesheetTextureID;

    //String key corresponds to the logical tile/entity ID
    //for dynamic sprites, frame no. must be appended to the base ID
    std::map<std::string, unsigned int> spriteIndexMap;
};

#endif