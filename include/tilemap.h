#ifndef U_TILEMAP
#define U_TILEMAP

#include <json-c/json.h>
#include <vector>

#include <windows.h>
#include <cstdint>

json_object* GetDataArray(const char* filePath)
{
    json_object *root = json_object_from_file(filePath);

    json_object *layers;
    json_object_object_get_ex(root, "layers", &layers);
    int array_len = json_object_array_length(layers);

    json_object *elem = json_object_array_get_idx(layers, 0);
    json_object *data;
    json_object_object_get_ex(elem, "data", &data);

    return data;
}

class FrameLimiter 
{
public:
    FrameLimiter(double targetFps = 60.0)
    {
        QueryPerformanceFrequency(&freq);
        frameDuration = int64_t(freq.QuadPart / targetFps);

        TIMECAPS tc;
        if (timeGetDevCaps(&tc, sizeof(tc)) == MMSYSERR_NOERROR)
            timeBeginPeriod(tc.wPeriodMin);  // Better sleep precision
    }

    ~FrameLimiter()
    {
        timeEndPeriod(1);
    }

    void markFrameStart()
    {
        QueryPerformanceCounter(&frameStart);
    }

    void sleepUntilNextFrame()
    {
        LARGE_INTEGER now;
        int64_t target = frameStart.QuadPart + frameDuration;
        int64_t spinThreshold = target - freq.QuadPart / 1000; // 1 ms

        while (true) {
            QueryPerformanceCounter(&now);
            if (now.QuadPart >= target)
                break;

            if (now.QuadPart < target - freq.QuadPart / 100) {
                Sleep(10);
            } else if (now.QuadPart < spinThreshold) {
                Sleep(1);
            }
        }
    }

private:
    LARGE_INTEGER freq{};
    LARGE_INTEGER frameStart{};
    int64_t frameDuration{};
};

#endif