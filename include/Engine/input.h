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

    //At the end of the frame, check every key, if it is down (1), then disable it in keyDownMap (0)
    void SetKeyDown()
    {
        lastFrameKeyMap = keyMap;
    }

    int GetKeyDown(int key)
    {
        return keyMap[key] == 1 && lastFrameKeyMap[key] == 0;
    }

private:
    std::array<int, 400> keyMap;
    std::array<int, 400> lastFrameKeyMap;
};

#endif