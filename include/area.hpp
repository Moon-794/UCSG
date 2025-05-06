#ifndef U_AREA
#define U_AREA

#include <cstdint>
#include <vector>
#include <json-c/json.h>

std::vector<bool> GenerateCollisionMap();

struct Layer
{
public:
    std::vector<int> tiles;
    unsigned int textureID;
};

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

struct Area
{
public:
    Area(std::string areaData)
    {
        ProcessTilemaps(areaData);

        layerSprite = Sprite(-1, glm::vec2(0, 0), nullptr);
        layerSprite.scale = glm::vec2(64 * 32, 64 * 32);
    }

    void ProcessTilemaps(std::string areaData)
    {
        std::string filePath("resources/areaData/" + areaData + "/" + areaData + ".json");
        json_object *root = json_object_from_file(filePath.c_str());

        json_object *layers;
        json_object_object_get_ex(root, "layers", &layers);
        int array_len = json_object_array_length(layers);

        for (size_t i = 0; i < array_len; i++)
        {
            json_object *elem = json_object_array_get_idx(layers, i);
            json_object *data;
            json_object_object_get_ex(elem, "data", &data);

            this->layers.push_back(ProcessAreaLayer(data));
        }
    }

    void DrawLayer(Renderer& renderer, int index)
    {
        layerSprite.textureID = layers[index].textureID;
        DrawSprite(renderer, layerSprite);
    }

    void SetShader(Shader* s)
    {
        layerSprite.shader = s;
    }

    int& GetTile(uint8_t x, uint8_t y)
    {
        return layers[0].tiles[(areaWidth * y) + x];
    }

private:
    std::vector<Layer> layers;
    std::vector<bool> collisionMap;

    Sprite layerSprite;
    
    unsigned int areaWidth;
    unsigned int areaHeight;
};

std::vector<bool> GenerateCollisionMap()
{
    return {false};
}

#endif