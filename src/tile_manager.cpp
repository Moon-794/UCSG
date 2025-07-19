#include "tile_manager.hpp"

TileManager::TileManager()
{
    std::cout << "Initialising Tile Manager..." << "\n";
    LoadTilesFromFile();
}

const Tile& TileManager::GetTile(const unsigned int tileID) const
{
    static Tile dummyTile;
    return dummyTile;
}

const Tile& TileManager::GetTile(const std::string& tileName) const
{
    static Tile dummyTile;
    return dummyTile;
}

void TileManager::LoadTilesFromFile()
{
    LoadBaseTiles();
}

void TileManager::LoadBaseTiles()
{
    int tileCount = 0;
    int tilesetCount = 0;

    const std::string resourcesPath = "resources/tilesets";

    if(fs::exists(resourcesPath))
    {
        if(!fs::is_directory(resourcesPath))
        {
            std::cout << "ERROR : " << resourcesPath << " found but it is not a folder!" << std::endl;
            return;
        }
    }
    else
    {
        std::cout << "ERROR : " << resourcesPath << " not found!" << std::endl;
        return;
    }

    for(const auto& entry : fs::directory_iterator(resourcesPath))
    {
        //Gives a linux + windows friendly folderpath
        std::string folderPath = std::regex_replace(entry.path().string(), std::regex("\\\\"), "/");
        LoadTileset(folderPath);
        tilesetCount++;
    }

    std::cout << "Loaded " << tileCount << " tiles from " << tilesetCount << " tilesets..." << std::endl;
}

void TileManager::LoadTileset(const std::string& path)
{
    const std::string jsonPath(path + "/data.json");

    if(fs::exists(jsonPath))
    {
        
    }
    else
    {
        std::cout << RED << "ERROR : data.json not found at: " << path << RESET << std::endl; 
    }
}