#include <stdio.h>
#include "string.h"

#include "../include/Engine.h"
#include "../include/Graphics.h"
#include "Timer.cpp"


// Input - Draw - Sons - Tile Editor

Vector2F cameraPos = Vector2F::zero;

float cameraSpeed = 200.f;

struct Matrix_2_3
{
    float values[2][3];
};

Vector2F matVecResult(Matrix_2_3 mat, Vector2F vec)
{
    Vector2F result = 
    {
        vec.x * mat.values[0][0] + vec.y * mat.values[0][1] + mat.values[0][2],
        vec.x * mat.values[1][0] + vec.y * mat.values[1][1] + mat.values[1][2]
    };

    return result;
};

Matrix_2_3 RotationMatrix(float angle)
{
    Matrix_2_3 mat =
    {
        .values = 
        {
            {cosf(angle / Calcul::TAU), - sinf(angle / Calcul::TAU), 0},
            {sinf(angle / Calcul::TAU),   cosf(angle / Calcul::TAU), 0}
        }
    };

    return mat;
};

// Matrix_2_3 MultipliedMatrix(Matrix_2_3 matA, Matrix_2_3 matB)
// {
//     Matrix_2_3 matResult = 
//     {
//         .values = 
//         {
//             {matA.values[0][0] * matB.values[0][0] + matA.values[0][1] * matB.values[1][0],
//              matA.values[0][0] * matB.values[]},
//             {}
//         }
//     };

//     return matResult;
// }

// kind of main for test
void example()
{
    Matrix_2_3 mat = 
    {
        .values = 
        {
            {1.f, 0.f, 10.f},
            {0.f, 1.f, 5.f},
        }
    };

    Vector2F vec{4, 2};

    Vector2F result = matVecResult(mat, vec);

    //printf("%f %f \n", result.x, result.y);
};

void GameFrame()
{
    example();

    // Graphics::DrawRect(Vector2F(sapp_width() / 2  -   8.f, sapp_height() / 2 - 60.f), 160.f, 120.f, Color::purple);
    // Graphics::DrawRect(Vector2F(0.25f,  0.25f), 0.5f, 0.5f, Color::cyan);

    // if (Input::KeyWasPressed(SAPP_KEYCODE_A))
    // {
    //     printf("Key Pressed\n");
    // }

    // Graphics::DrawQuad(Vector2F::zero, Vector2F(343.f, 50.f), Vector2F(260.f, 418.f), Vector2F(118.f, 350.f), Color::green);
    // Graphics::DrawCircle(Vector2F(sapp_width() / 2.f, sapp_height() / 2.f), 50.f, 50.f, Color::red);
    // Graphics::DrawLine(Vector2F(0.f, sapp_height()), Vector2F(sapp_width(), 0.f), 10.f, Color::blue);
    
    // Graphics::DrawRect(Vector2F(Input::mousePos.x - 32.f, Input::mousePos.y - 32.f), 64.f, 64.f, Color::yellow);

    // Graphics::DrawLine(Vector2F(0, sapp_height()), Input::mousePos, 5.f, Color::red);

    // Graphics::Translate(cameraPos);

    // if(Input::KeyBeingPressed(SAPP_KEYCODE_W))
    // {
    //     cameraPos += Vector2F::down * cameraSpeed * Timer::deltaTime;
    // }
    // else if(Input::KeyBeingPressed(SAPP_KEYCODE_D))
    // {
    //     cameraPos += Vector2F::right * cameraSpeed * Timer::deltaTime;
    // }
    // else if(Input::KeyBeingPressed(SAPP_KEYCODE_S))
    // {
    //     cameraPos += Vector2F::up * cameraSpeed * Timer::deltaTime;
    // }
    // else if(Input::KeyBeingPressed(SAPP_KEYCODE_A))
    // {
    //     cameraPos += Vector2F::left * cameraSpeed * Timer::deltaTime;
    // }

    // Graphics::DrawTile(Vector2F(0 * 8, 0), 8, 8, Color::baseImg, 0.f, 0.5f, 0.f, 0.5f);
    // Graphics::DrawTile(Vector2F(1 * 8, 0), 8, 8, Color::baseImg, 0.5f, 1.f, 0.f, 0.5f);
    // Graphics::DrawTile(Vector2F(2 * 8, 0), 8, 8, Color::baseImg, 0.f, 0.5f, 0.5f, 1.f);
    // Graphics::DrawTile(Vector2F(3 * 8, 0), 8, 8, Color::baseImg, 0.5f, 1.f, 0.5f, 1.f);

    Timer::Tick();
}

struct GameState 
{
    int frame_count;
};

GameState* game_state = 0;

#ifdef __cplusplus // If used by C++ code, 
extern "C"         // we need to export the C interface
{          
    #if _WIN32
    #define EXPORT __declspec(dllexport)
    #else
    #define EXPORT
    #endif

    EXPORT void GameUpdate(void* memory)
    {
        game_state = (GameState*) memory;

        printf("YES : %i\n", game_state->frame_count);
        game_state->frame_count++;
    }

    EXPORT void DLL_GameFrame()
    {
        example();

        // Graphics::DrawRect(Vector2F(sapp_width() / 2  -   8.f, sapp_height() / 2 - 60.f), 160.f, 120.f, Color::purple);
        // Graphics::DrawRect(Vector2F(0.25f,  0.25f), 0.5f, 0.5f, Color::cyan);

        // if (Input::KeyWasPressed(SAPP_KEYCODE_A))
        // {
        //     printf("Key Pressed\n");
        // }

        // Graphics::DrawQuad(Vector2F::zero, Vector2F(343.f, 50.f), Vector2F(260.f, 418.f), Vector2F(118.f, 350.f), Color::green);
        // Graphics::DrawCircle(Vector2F(sapp_width() / 2.f, sapp_height() / 2.f), 50.f, 50.f, Color::red);
        // Graphics::DrawLine(Vector2F(0.f, sapp_height()), Vector2F(sapp_width(), 0.f), 10.f, Color::blue);
        
        // Graphics::DrawRect(Vector2F(Input::mousePos.x - 32.f, Input::mousePos.y - 32.f), 64.f, 64.f, Color::yellow);

        // Graphics::DrawLine(Vector2F(0, sapp_height()), Input::mousePos, 5.f, Color::red);

        // Graphics::Translate(cameraPos);

        // if(Input::KeyBeingPressed(SAPP_KEYCODE_W))
        // {
        //     cameraPos += Vector2F::down * cameraSpeed * Timer::deltaTime;
        // }
        // else if(Input::KeyBeingPressed(SAPP_KEYCODE_D))
        // {
        //     cameraPos += Vector2F::right * cameraSpeed * Timer::deltaTime;
        // }
        // else if(Input::KeyBeingPressed(SAPP_KEYCODE_S))
        // {
        //     cameraPos += Vector2F::up * cameraSpeed * Timer::deltaTime;
        // }
        // else if(Input::KeyBeingPressed(SAPP_KEYCODE_A))
        // {
        //     cameraPos += Vector2F::left * cameraSpeed * Timer::deltaTime;
        // }

        // Graphics::DrawTile(Vector2F(0 * 8, 0), 8, 8, Color::baseImg, 0.f, 0.5f, 0.f, 0.5f);
        // Graphics::DrawTile(Vector2F(1 * 8, 0), 8, 8, Color::baseImg, 0.5f, 1.f, 0.f, 0.5f);
        // Graphics::DrawTile(Vector2F(2 * 8, 0), 8, 8, Color::baseImg, 0.f, 0.5f, 0.5f, 1.f);
        // Graphics::DrawTile(Vector2F(3 * 8, 0), 8, 8, Color::baseImg, 0.5f, 1.f, 0.5f, 1.f);

        Timer::Tick();
    }
}
#endif