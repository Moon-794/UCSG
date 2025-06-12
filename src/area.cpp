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

unsigned int LoadImageData(const std::string& areaName, ImageData& data)
{
    stbi_set_flip_vertically_on_load(true);
    std::string path("resources/areaData/" + areaName + "/" + areaName + ".png");
    stbi_uc* image = stbi_load(path.c_str(), &data.width, &data.height, &data.nrChannels, 4);
    
    if(!image)
        return -1;

    data.data = image;
    return 0;
}

//Generates a list of collider values based on the area tilemap, only layer 0 is considered when generating the collision map
std::map<int, bool> GenerateCollisionMap(std::string areaData)
{
    std::map<int,bool> colMap;

    std::string filePath("resources/areaData/" + areaData + "/Tileset.json");
    json_object *root = json_object_from_file(filePath.c_str());

    json_object *tiles;
    json_object_object_get_ex(root, "tiles", &tiles);
    int array_len = json_object_array_length(tiles);

    //Loop through each 
    for (size_t i = 0; i < array_len; i++)
    {
        json_object *elem = json_object_array_get_idx(tiles, i);
        json_object *properties;
        json_object_object_get_ex(elem, "properties", &properties);

        json_object *collider = json_object_array_get_idx(properties, 0);
        json_object *value;
        json_object_object_get_ex(collider, "value", &value);

        bool val = json_object_get_boolean(value);

        colMap.insert(std::pair<int, bool>(i, val));
    }
    
    return colMap;
}

AreaManager::AreaManager(std::shared_ptr<Shader> areaShader)
{
    std::cout << "Initialising Area Manager" << "\n";

    this->areaShader = areaShader;

    areas = AreaManager::LoadAllAreas();
    currentArea = areas["TestMap"];
}

std::shared_ptr<AreaData> AreaManager::GetCurrentArea()
{
    return currentArea;
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
        std::string areaName = folderPath.erase(0, 19);
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
    std::vector<TileLayer> layers = LoadLayers(areaName);
    
    for (size_t i = 0; i < layers.size(); i++)
    {
        unsigned int textureID = GenerateLayerTextureID(areaName, layers[i].layerData, tileWidth, areaWidth, areaHeight);
        Sprite layerSprite(textureID, glm::vec2(0, 0), areaShader);
        layerSprite.scale = glm::vec2(areaWidth, areaHeight);

        layers[i].layerSprite = layerSprite;
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

std::unordered_map<unsigned int, Tile> AreaManager::LoadTileset(std::string areaName)
{
    std::unordered_map<unsigned int, Tile> tileset;

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
        tileset.insert({tileID, tile});
    }

    return tileset;
}

std::vector<TileLayer> AreaManager::LoadLayers(std::string areaName)
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
            LoadTileLayer(tileLayer, data);

            tileLayers.push_back(tileLayer);
        }
    }

    return tileLayers;
}

unsigned int AreaManager::LoadTileLayer(TileLayer& tileLayer, const json_object* element)
{
    tileLayer.layerData = std::vector<std::vector<int>>(32, std::vector<int>(32));

    for (size_t tx = 0; tx < 32; tx++)
    {
        for (size_t ty = 0; ty < 32; ty++)
        {
            json_object* tile = json_object_array_get_idx(element, (ty * 32) + tx);
            int val = json_object_get_int(tile) - 1;
            tileLayer.layerData[tx][ty] = val;
        }
    }

    return 0;
}

unsigned int AreaManager::GenerateLayerTextureID(const std::string& areaName, const std::vector<std::vector<int>>& tileIDS, int spriteWidth, int mapWidth, int mapHeight)
{
    const int NUM_CHANNELS = 4;

    ImageData imageData;
    if(LoadImageData(areaName, imageData) == -1)
    {
        std::cout << "Failed to load image data\n";
    }

    //Num sprites in image + 1 for empty sprite (tiledID 0)
    int numImages = (imageData.width / spriteWidth);

    //Extract each individual texture from the tileset
    std::vector<unsigned char*> subImages(numImages);
    for (size_t n = 0; n < numImages; n++)
    {
        subImages[n] = new unsigned char[spriteWidth * spriteWidth * NUM_CHANNELS] {0}; 
        for (size_t x = 0; x < spriteWidth; x++)
        {
            for (size_t y = 0; y < spriteWidth; y++)
            { 
                int parentOffset = ((y * imageData.width) + x + (spriteWidth * n)) * NUM_CHANNELS;
                int childOffset = ((y * spriteWidth) + x) * NUM_CHANNELS;

                subImages[n][childOffset + 0] = imageData.data[parentOffset + 0];
                subImages[n][childOffset + 1] = imageData.data[parentOffset + 1];
                subImages[n][childOffset + 2] = imageData.data[parentOffset + 2];
                subImages[n][childOffset + 3] = imageData.data[parentOffset + 3];    
            }
        }
    }

    //Empty texture (black square)
    unsigned char emptyImage[spriteWidth * spriteWidth * NUM_CHANNELS] = {0};

    //Begin stitching together tilemap
    unsigned char* tileMapImage = new unsigned char[(mapWidth * spriteWidth) * (mapHeight * spriteWidth) * NUM_CHANNELS];

    for (size_t x = 0; x < mapWidth; x++)
    {
        for (size_t y = 0; y < mapHeight; y++)
        {
            int tileID = tileIDS[x][y];

            unsigned char* dataSource = (tileID == -1) ? emptyImage : subImages[tileID];

            // Copy subImage to tileMapImage
            for (size_t i = 0; i < spriteWidth; i++) // tile row
            {
                for (size_t j = 0; j < spriteWidth; j++) // tile column
                {
                    // Where we're writing into the final image (top-down layout)
                    int pixelX = x * 16 + j; // horizontal pixel in tilemap
                    int pixelY = (31 - y) * 16 + i; // vertical pixel in tilemap

                    int mapOffset = (pixelY * 512 + pixelX) * 4;
                    int subOffset = (i * 16 + j) * 4;

                    //if(tileID == 2 && i == 0 && j == 0)
                        //std::cout << (int)dataSource[subOffset + 0] << " " << (int)dataSource[subOffset + 1] << " " << (int)dataSource[subOffset + 2] << " " << (int)dataSource[subOffset + 3] << "\n";

                    tileMapImage[mapOffset + 0] = dataSource[subOffset + 0];
                    tileMapImage[mapOffset + 1] = dataSource[subOffset + 1];
                    tileMapImage[mapOffset + 2] = dataSource[subOffset + 2];
                    tileMapImage[mapOffset + 3] = dataSource[subOffset + 3];

                }
            }
        }
    }

    //This can have its own function
    unsigned int texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (mapWidth * spriteWidth), (mapHeight * spriteWidth), 0, GL_RGBA, GL_UNSIGNED_BYTE, tileMapImage);

    //Delete image data
    for(int i = 0; i < subImages.size(); i++)
    {
        delete(subImages[i]);
    }

    delete(tileMapImage);
    stbi_image_free(imageData.data);

    return texture;
}