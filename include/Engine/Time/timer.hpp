#ifndef U_TIMER
#define U_TIMER

#include <functional>
#include <string>

class Timer
{
public:
    Timer(int tickLength, std::function<void()> callback = nullptr, bool isEnabled = true, bool repeats = false);
    
    void Tick();
    void Enable();
    void Disable();
    void Toggle();
    void Reset(bool enable = true);

    float GetProgress();

private:
    int currentTick = 0;
    int ticksRequired = -1;
    bool complete = false;

    bool isEnabled = true;
    bool repeat = false;

    std::function<void()> OnCompleteCallback;
};

#endif