#include <stdio.h>

#include "../include/Engine.h"
#include "../include/Graphics.h"
#include "Timer.cpp"

Vector2F FloatToVertex(Vector2F pos)
{
    Vector2F vertexPos;

    vertexPos.x =   pos.x * 2 / sapp_width()  - 1;
    vertexPos.y = -(pos.y * 2 / sapp_height() - 1);

    // if (pos.x < sapp_width() / 2.f)
    // {
    //     vertexPos.x -= 1;
    // }
    // if (pos.y < sapp_height() / 2.f)
    // {
    //     vertexPos.y -= 1;
    // }

    return vertexPos;
}

void GameFrame()
{
    //printf("%f %f \n", Input::mouseX, Input::mouseY);    

    Vector2F test = FloatToVertex(Vector2F(Input::mouseX, Input::mouseY));

    printf("%f %f\n", test.x, test.y);

    Vector2F mousePos = FloatToVertex(Vector2F(Input::mouseX, Input::mouseY));

    Graphics::DrawRect(Vector2F(mousePos.x - 0.25f, mousePos.y - 0.25f), 0.5f, 0.5f, Color::yellow);
    Graphics::DrawRect(Vector2F(-0.75f, -0.75f), 0.5f, 0.5f, Color::purple);
    Graphics::DrawRect(Vector2F( 0.25f,  0.25f), 0.5f, 0.5f, Color::cyan);

    if (Timer::totalTime <= 2.0)
    {
        Graphics::DrawCircle(Vector2F::zero, 0.5f, 50, Color::red);

        Graphics::DrawQuad(Vector2F::zero, Vector2F(0.34f, -0.5f), Vector2F(0.5f, -0.5f), Vector2F(0.23f, 0.9f), Color::green);

        Graphics::DrawLine(Vector2F::one, Vector2F::one * -1, 0.009f, Color::blue);

        Graphics::DrawTriangle(Vector2F::zero, Vector2F(0.5f, 1.f), Vector2F(1.f, 0.f), Color::purple);
    }
    
    Timer::Tick();
}