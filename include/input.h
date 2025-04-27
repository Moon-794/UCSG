#ifndef U_INPUT
#define U_INPUT

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
    std::array<int, 90> keyMap;
};

#endif