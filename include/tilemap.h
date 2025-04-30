#ifndef U_TILEMAP
#define U_TILEMAP

#include <json-c/json.h>
#include <vector>

#include <windows.h>
#include <cstdint>

json_object* GetDataArray(const char* filePath, int layer)
{
    json_object *root = json_object_from_file(filePath);

    json_object *layers;
    json_object_object_get_ex(root, "layers", &layers);
    int array_len = json_object_array_length(layers);

    json_object *elem = json_object_array_get_idx(layers, layer);
    json_object *data;
    json_object_object_get_ex(elem, "data", &data);

    return data;
}

#endif