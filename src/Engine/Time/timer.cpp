#include "Engine/Time/timer.hpp"

Timer::Timer(int tickLength, std::function<void()> callback, bool isEnabled, bool repeats)
{
    this->ticksRequired = tickLength;
    this->OnCompleteCallback = callback;
    this->isEnabled = isEnabled;
    this->repeat = repeats;
}

//Step forward timer by 1 tick, if ``currentTick == ticksRequired``, mark timer as complete and call ``OnComplete``
void Timer::Tick()
{
    if(!isEnabled)
        return;
    
    //Timer Complete!
    if(currentTick == ticksRequired)
    {
        if(repeat)
        {
            currentTick = 0;
        }
        else
        {
            complete = true;
        }

        if(OnCompleteCallback)
            OnCompleteCallback();
    }
    else
    {
        currentTick++;
    }
}

void Timer::Enable()
{
    isEnabled = true;
}

void Timer::Disable()
{
    isEnabled = false;
}

//Toggle 
void Timer::Toggle()
{
    isEnabled = !isEnabled;
}

void Timer::Reset(bool enable)
{
    currentTick = 0;
    isEnabled = enable;
}

float Timer::GetProgress()
{
    return (float)currentTick/(float)ticksRequired;
}
