#include "Engine/Time/time_manager.hpp"

TimeManager::TimeManager()
{
    std::cout << "Time Manager Initialised!" << std::endl;   
}

std::shared_ptr<Timer> TimeManager::AddTimer(std::string name, int tickLength, std::function<void()> callback, bool isEnabled, bool repeats)
{
    std::shared_ptr<Timer> timer = std::make_shared<Timer>(tickLength, callback, isEnabled, repeats);
    timers.insert({name, timer});

    return timer;
}

void TimeManager::RemoveTimer(const std::string& name)
{
    auto it = timers.find(name);
    timers.erase(it);
}

void TimeManager::ProcessTimers()
{
    globalTicks++;

    for (const auto& x : timers) 
    {
        x.second->Tick();
    }
}

int TimeManager::GetGlobalTicks()
{
    return globalTicks;
}