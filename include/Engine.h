#pragma once

struct sapp_event;

namespace Input
{
    extern float mouseX, mouseY;

    void mouse_position_callback(const sapp_event* event);
};