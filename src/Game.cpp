#include <stdio.h>

#include "../include/Engine.h"
#include "../include/Graphics.h"
#include "Timer.cpp"

void GameFrame()
{
    Graphics::DrawRect(Vector2F(Input::mousePos.x - 100.f, Input::mousePos.y - 50.f), 200.f, 100.f, Color::yellow);
    Graphics::DrawRect(Vector2F(sapp_width() / 2  -  80.f, sapp_height() / 2 - 60.f), 160.f, 120.f, Color::purple);
    Graphics::DrawRect(Vector2F( 0.25f,  0.25f), 0.5f, 0.5f, Color::cyan);

    if (Input::keys[SAPP_KEYCODE_A])
    {
        printf("yoooo\n");
    }

    if (Timer::totalTime <= 2.0)
    {
        Graphics::DrawQuad(Vector2F::zero, Vector2F(343.f, 50.f), Vector2F(260.f, 418.f), Vector2F(118.f, 350.f), Color::green);

        Graphics::DrawCircle(Vector2F(sapp_width() / 2.f, sapp_height() / 2.f), 50.f, 50.f, Color::red);

        Graphics::DrawLine(Vector2F(0.f, sapp_height()), Vector2F(sapp_width(), 0.f), 5.f, Color::blue);
    }
    
    Timer::Tick();
}