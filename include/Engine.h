#pragma once

#include "Math.h"

struct sapp_event;

namespace Input
{
    extern bool keys[256];

    extern Vector2F mousePos;

    void KeyDownCallback(const sapp_event* event);
    void KeyUpCallback  (const sapp_event* event);

    void MousePositionCallback(const sapp_event* event);
};