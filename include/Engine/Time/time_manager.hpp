#ifndef U_TIME_MANAGER
#define U_TIME_MANAGER

#include "Engine/Time/timer.hpp"

#include <memory>
#include <map>
#include <iostream>

class TimeManager
{
public:
    TimeManager();
    
    std::shared_ptr<Timer> AddTimer(std::string name, int tickLength, std::function<void()> callback = {}, bool isEnabled = true, bool repeats = false);
    void RemoveTimer(const std::string& name);

    int GetGlobalTicks();

private:
    //Collection of all timers
    std::map<std::string, std::shared_ptr<Timer>> timers;

    //Variable to keep track of how many ticks the engine has been running for
    int globalTicks = 0;

    void ProcessTimers();

    friend class Engine;
};

#endif