#ifndef U_TILE_MANAGER
#define U_TILE_MANAGER

#include <map>
#include <vector>
#include <string>
#include <filesystem>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;

//All tiles are loaded at startup from the resources/tilesets folder
//All Tiles are stored in one container with unique ID's
//Tile information can be retrieved via the global index (not tileset index), or the tile name

//Animated tiles are simply an array of continguous tile frames, each frame of the animation has its own ID, name
//and properties. Animated tiles always play at a fixed speed.

#define RESET   "\033[0m"
#define RED     "\033[31m"

struct Tile
{
    unsigned int id;
    std::string tileName;

    std::map<std::string, std::string> properties;
};

class TileManager
{
public:
    TileManager();
    const Tile& GetTile(const unsigned int tileID) const;
    const Tile& GetTile(const std::string& tileName) const;
private:
    std::map<std::string, unsigned int> nameToID;
    std::vector<Tile> tiles;

    void LoadTilesFromFile();
    void LoadBaseTiles();
    void LoadTileset(const std::string& path);
};



#endif