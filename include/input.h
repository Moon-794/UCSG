#ifndef U_INPUT
#define U_INPUT

#include "renderer.hpp"
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
    std::array<int, 160> keyMap;
};

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#endif