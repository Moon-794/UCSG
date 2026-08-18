#ifndef U_INPUT
#define U_INPUT

#include "Engine/Graphics/renderer.hpp"
#include <array>

class InputMap
{
public:

    int GetKey(int key)
    {
        return keyMap[key];
    }

    void SetKey(int key, int status)
    {
        keyMap[key] = status;
    }

private:
    std::array<int, 400> keyMap;
};

#endif