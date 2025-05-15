#include "area.hpp"

Layer ProcessAreaLayer(json_object* layerData)
{
    int data_len = json_object_array_length(layerData);

    std::vector<int> tileIDS(32 * 32);
    int width = 32;
    int height = 32;
    int nrChannels = 0;
    int spriteWidth = 16;

    for (size_t h = 0; h < 32 * 32; h++)
    {
        json_object* tile = json_object_array_get_idx(layerData, h);
        tileIDS[h] = json_object_get_int(tile) - 1;
    }

    stbi_set_flip_vertically_on_load(false);
    stbi_uc* image = stbi_load("resources/areaData/TestMap/TestMap.png", &width, &height, &nrChannels, 4);
    int length = width * height * 4;

    //Num sprites in image + 1 for empty sprite (tiledID 0)
    int numImages = (width / spriteWidth);
    
    std::vector<unsigned char*> subImages(numImages);
    unsigned char emptyImage[16 * 16 * 4];
    
    //Create Empty Image //TODO: Make this a constant variable somewhere relevant
    for (size_t x = 0; x < 16; x++)
    {
        for (size_t y = 0; y < 16; y++)
        {
            int offset = ((x * 16) + y) * 4;
            emptyImage[offset + 0] = 0;
            emptyImage[offset + 1] = 0;
            emptyImage[offset + 2] = 0;
            emptyImage[offset + 3] = 0;
        }
    }

    for (size_t n = 0; n < numImages; n++)
    {
        subImages[n] = new unsigned char[16 * 16 * 4]; 
        for (size_t x = 0; x < 16; x++)
        {
            for (size_t y = 0; y < 16; y++)
            {
                int parentOffset = ((x * 80) + y + (spriteWidth * n)) * 4;
                int childOffset = ((x * 16) + y) * 4;
                subImages[n][childOffset + 0] = image[parentOffset + 0];
                subImages[n][childOffset + 1] = image[parentOffset + 1];
                subImages[n][childOffset + 2] = image[parentOffset + 2];
                subImages[n][childOffset + 3] = image[parentOffset + 3];
            }
        }
    }
    
    //Begin stitching together tilemap
    int mapWidth = 32;
    int mapHeight = 32;
    unsigned char* tileMapImage = new unsigned char[512 * 512 * 4];

    for (size_t x = 0; x < mapWidth; x++)
    {
        for (size_t y = 0; y < mapHeight; y++)
        {
            int tileID = tileIDS[(x * mapWidth) + y];

            unsigned char* dataSource;
            if(tileID == -1)
            {
                dataSource = emptyImage;
            }
            else
            {
                dataSource = subImages[tileID];
            }

            //Copy subImage to tilemapImage
            for (size_t i = 0; i < 16; i++)
            {
                for (size_t j = 0; j < 16; j++)
                {
                    //Move down 16 lines for each y
                    int mapOffsetY = (512 * 512 * 4) - ((x * 16) + i) * (16 * mapWidth * 4);
                    //Move down 1 line per j
                    int subOffsetY = i * 16 * 4;
                    
                    //Move 16 pixels across for each x
                    int mapOffsetX = y * (16 * 4);
                    int subOffsetX = j * 4;

                    int subOffset = subOffsetY + subOffsetX;
                    int mapOffset = mapOffsetY + mapOffsetX;

                    tileMapImage[mapOffset + subOffsetX + 0] = dataSource[subOffset + 0];
                    tileMapImage[mapOffset + subOffsetX + 1] = dataSource[subOffset + 1];
                    tileMapImage[mapOffset + subOffsetX + 2] = dataSource[subOffset + 2];
                    tileMapImage[mapOffset + subOffsetX + 3] = dataSource[subOffset + 3];
                }
            }
        }
    }

    unsigned int texture = 0;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, tileMapImage);

    //Free stb data
    
    //Delete image data
    for(int i = 0; i < subImages.size(); i++)
    {
        delete(subImages[i]);
    }

    delete(tileMapImage);
    stbi_image_free(image);

    Layer areaLayer;
    areaLayer.textureID = texture;
    areaLayer.tiles = tileIDS;

    return areaLayer;
}

//Generates a list of collider values based on the area tilemap, only layer 0 is considered when generating the collision map
std::map<int, bool> GenerateCollisionMap(std::string areaData)
{
    std::map<int,bool> colMap;

    std::string filePath("resources/areaData/" + areaData + "/" + areaData + "Tileset.json");
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
