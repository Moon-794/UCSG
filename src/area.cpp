#include "area.hpp"

int JSONGetInt(json_object* obj, std::string fieldName)
{
    json_object* value;
    json_object_object_get_ex(obj, fieldName.c_str(), &value);

    if(value == nullptr)
    {
        std::cout << "Fieldname \"" << fieldName << "\" not found." << std::endl;
        return -1;
    }

    return json_object_get_int(value);
}

json_object* GetRootAreaDataFromFile(std::string areaName)
{
    std::string filePath("resources/areaData/" + areaName + "/data.json");
    json_object *root = json_object_from_file(filePath.c_str());
    return root;
}

AreaManager::AreaManager()
{
    std::cout << "Initialising Area Manager" << "\n";

    areas = AreaManager::LoadAllAreas();
    currentArea = areas["Hallway"];
}

void AreaManager::ForceTransition(std::string areaName, glm::vec2 spawnPosition, float& playerX, float& playerY)
{
    currentArea = areas[areaName];
    playerX = spawnPosition.x;
    playerY = spawnPosition.y;
}

const AreaData& AreaManager::getCurrentArea() const
{
    return *currentArea;
}

std::unordered_map<std::string, std::shared_ptr<AreaData>> AreaManager::LoadAllAreas()
{
    std::unordered_map<std::string, std::shared_ptr<AreaData>> areas;

    //Get all area folders in resources/areaData
    std::string resourcesPath = "resources/areaData";
    for(const auto& entry : fs::directory_iterator(resourcesPath))
    {
        //Gives a linux + windows friendly folderpath
        std::string folderPath = std::regex_replace(entry.path().string(), std::regex("\\\\"), "/");

        //Erase the first part of the path to use in file names
        std::string areaName(entry.path().filename().string());
        AreaData area = LoadAreaData(areaName);
        areas.insert({areaName, std::make_shared<AreaData>(std::move(area))});
    }

    return areas;
}

//Loads an area from disk, filepath is areaData folder (e.g "resources/areaData/[AREANAME]")
AreaData AreaManager::LoadAreaData(std::string areaName)
{
    AreaData area;

    json_object* root = GetRootAreaDataFromFile(areaName);
    int areaWidth = JSONGetInt(root, "width");
    int areaHeight = JSONGetInt(root, "height");

    if((areaWidth <= 0 || areaWidth >= 128) && (areaHeight <= 0 || areaHeight >= 128))
    {
        std::cout << "Error: Area width and height must be between 1 and 127.\n";
        std::cout << "AreaWidth: " << areaWidth << " " << "AreaHeight: " << areaHeight << std::endl;
        return area;
    }

    //Area dimensions
    area.areaWidth = areaWidth;
    area.areaHeight = areaHeight;

    //Area Identifiers
    //Unique ID given out by areaManager
    area.areaID = nextAvailableID;
    nextAvailableID++;
    area.areaName = areaName;

    //Tile information
    int tileWidth = JSONGetInt(root, "tilewidth");
    int tileHeight = JSONGetInt(root, "tileheight");
    area.tileWidth = tileWidth;
    area.tileHeight = tileHeight;


    //Some properties are parsed directly into member variables, based on whether they will change
    std::unordered_map<std::string, std::string> properties = LoadAreaProperties(areaName);

    //Use areaname property for name, if it doesnt exist just use the resources folder directory name
    if(properties.count("areaName"))
        area.areaName = properties["areaName"];

    //Properties, can contain anything
    area.properties = properties;

    //Tileset
    area.tileset = LoadTileset(areaName);

    //Layers
    std::vector<TileLayer> layers = LoadLayers(areaName, areaWidth, areaHeight);
    
    for (size_t i = 0; i < layers.size(); i++)
    {
        GenerateTileLayerCollisionMap(layers[i], area.tileset, areaWidth, areaHeight);
    }
    
    area.tileLayers = layers;
    return area;    
}

std::unordered_map<std::string, std::string> AreaManager::LoadAreaProperties(std::string areaName)
{
    std::unordered_map<std::string, std::string> properties;
    json_object* root = GetRootAreaDataFromFile(areaName);

    json_object *propertyArray;
    json_object_object_get_ex(root, "properties", &propertyArray);
    int array_len = json_object_array_length(propertyArray);

    for (size_t i = 0; i < array_len; i++)
    {
        json_object *elem = json_object_array_get_idx(propertyArray, i);
        json_object *subElement;

        json_object_object_get_ex(elem, "name", &subElement);
        std::string propertyName(json_object_get_string(subElement));

        json_object_object_get_ex(elem, "value", &subElement);
        std::string propertyValue(json_object_get_string(subElement));

        properties.insert({propertyName, propertyValue});
    }

    return properties;
}

std::vector<Tile> AreaManager::LoadTileset(std::string areaName)
{
    std::vector<Tile> tileset;

    std::string filePath("resources/areaData/" + areaName + "/Tileset.json");
    json_object *root = json_object_from_file(filePath.c_str());

    json_object *tileArray;
    json_object_object_get_ex(root, "tiles", &tileArray);
    int array_len = json_object_array_length(tileArray);
    
    //Loop through Tiles
    for (size_t i = 0; i < array_len; i++)
    {
        Tile tile;

        json_object *tile_obj = json_object_array_get_idx(tileArray, i);
        json_object *elem;
        json_object_object_get_ex(tile_obj, "type", &elem);
        
        tile.tileType = std::string(json_object_get_string(elem));
        unsigned int tileID = JSONGetInt(tile_obj, "id");

        json_object* propertyArray;
        json_object_object_get_ex(tile_obj, "properties", &propertyArray);
        int prop_array_len = json_object_array_length(propertyArray);

        //Loop through properties of each tile
        std::unordered_map<std::string, std::string> tileProperties;
        for (size_t j = 0; j < prop_array_len; j++)
        {
            json_object *elem = json_object_array_get_idx(propertyArray, j);
            json_object *subElement;

            json_object_object_get_ex(elem, "name", &subElement);
            std::string propertyName(json_object_get_string(subElement));

            json_object_object_get_ex(elem, "value", &subElement);
            std::string propertyValue(json_object_get_string(subElement));

            tileProperties.insert({propertyName, propertyValue});
        }

        tile.tileProperties = tileProperties;
        tileset.push_back(tile);
    }

    return tileset;
}

std::vector<TileLayer> AreaManager::LoadLayers(std::string areaName, int areaWidth, int areaHeight)
{
    std::vector<TileLayer> tileLayers;
    json_object* root = GetRootAreaDataFromFile(areaName);

    json_object* layerArray;
    json_object_object_get_ex(root, "layers", &layerArray);
    int array_len = json_object_array_length(layerArray);
    
    for (size_t i = 0; i < array_len; i++)
    {
        json_object *elem = json_object_array_get_idx(layerArray, i);

        json_object* layerType;
        json_object_object_get_ex(elem, "type", &layerType);
        std::string tileLayerType = std::string(json_object_get_string(layerType));
        
        if(tileLayerType == "tilelayer")
        {
            json_object *data;
            json_object_object_get_ex(elem, "data", &data);

            TileLayer tileLayer;
            LoadTileLayer(tileLayer, data, areaWidth, areaHeight);

            tileLayers.push_back(tileLayer);
        }
    }

    return tileLayers;
}

unsigned int AreaManager::LoadTileLayer(TileLayer& tileLayer, const json_object* element, int areaWidth, int areaHeight)
{
    tileLayer.layerData = std::vector<std::vector<int>>(areaWidth, std::vector<int>(areaHeight));

    for (size_t tx = 0; tx < areaWidth; tx++)
    {
        for (size_t ty = 0; ty < areaHeight; ty++)
        {
            json_object* tile = json_object_array_get_idx(element, (ty * areaWidth) + tx);
            int val = json_object_get_int(tile) - 1;
            tileLayer.layerData[tx][ty] = val;
        }
    }

    return 0;
}

void AreaManager::GenerateTileLayerCollisionMap(TileLayer& layer, const Tileset& tileset, const int areaWidth, const int areaHeight)
{
    for (size_t i = 0; i < tileset.size(); i++)
    {
        bool isCollider = GetTilePropertyBool(tileset[i], "collider");
        layer.collisionMap.insert({i, isCollider});
    }
}

std::string GetTilePropertyString(const Tile& tile, const std::string& key)
{
    if(tile.tileProperties.find(key) == tile.tileProperties.end())
    {
        std::cout << "Failed to find tile property for tile: " << tile.tileType << ". Key: " << key << "\n";
        return "null";
    }

    return tile.tileProperties.at(key); 
}

bool GetTilePropertyBool(const Tile& tile, const std::string& key)
{
    std::string value = GetTilePropertyString(tile, key);

    if(value == "null")
    {
        std::cout << "Failed to convert tile property to bool: " << tile.tileType << ". Key: " << key << " Value: " << value <<  ", returning false.\n";
        return false;
    }
    else
    {
        return value == "true";
    }
}