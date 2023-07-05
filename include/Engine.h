#pragma once

#include "Math.h"

struct sapp_event;
enum sapp_keycode;

namespace Input
{
    extern bool keyStates[256];
    extern bool previousKeyStates[256];

    extern Vector2F mousePos;

    void KeyDownCallback(const sapp_event* event);
    void KeyUpCallback  (const sapp_event* event);

    void MouseMoveCallback(const sapp_event* event);

    bool KeyBeingPressed(sapp_keycode key);
    bool KeyWasPressed(sapp_keycode key);
    bool KeyWasReleased(sapp_keycode key);
};