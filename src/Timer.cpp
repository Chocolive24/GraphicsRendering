#include "sokol_app.h"

namespace Timer
{
    double deltaTime, totalTime;

    void Tick()
    {
        deltaTime = sapp_frame_duration();
        totalTime += deltaTime;
    }  
}