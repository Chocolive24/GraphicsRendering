#include <stdio.h>
#include "string.h"

#include "../include/Engine.h"
#include "../include/Graphics.h"
#include "Timer.cpp"

void GameFrame()
{
    
    // Graphics::DrawRect(Vector2F(sapp_width() / 2  -   80.f, sapp_height() / 2 - 60.f), 160.f, 120.f, Color::purple);
    // Graphics::DrawRect(Vector2F(0.25f,  0.25f), 0.5f, 0.5f, Color::cyan);

    // if (Input::KeyWasPressed(SAPP_KEYCODE_A))
    // {
    //     printf("Key Pressed\n");
    // }

    // Graphics::DrawQuad(Vector2F::zero, Vector2F(343.f, 50.f), Vector2F(260.f, 418.f), Vector2F(118.f, 350.f), Color::green);
    // Graphics::DrawCircle(Vector2F(sapp_width() / 2.f, sapp_height() / 2.f), 50.f, 50.f, Color::red);
    // Graphics::DrawLine(Vector2F(0.f, sapp_height()), Vector2F(sapp_width(), 0.f), 10.f, Color::blue);
    
    Graphics::DrawRect(Vector2F(Input::mousePos.x - 32.f, Input::mousePos.y - 32.f), 64.f, 64.f, Color::yellow);

    Graphics::DrawTile(Vector2F(0 * 80, 0), 80, 80, Color::white, 0.f, 0.5f, 0.f, 0.5f);
    Graphics::DrawTile(Vector2F(1 * 80, 0), 80, 80, Color::white, 0.5f, 1.f, 0.f, 0.5f);
    Graphics::DrawTile(Vector2F(2 * 80, 0), 80, 80, Color::white, 0.f, 0.5f, 0.5f, 1.f);
    Graphics::DrawTile(Vector2F(3 * 80, 0), 80, 80, Color::white, 0.5f, 1.f, 0.5f, 1.f);

    Timer::Tick();
}