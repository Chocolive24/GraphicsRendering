#pragma once 

#include "../include/Math.h"

struct Vertex
{
    float x, y, z;
    float r, g, b, a;
};

struct Color
{
    float r, g, b, a;

    Color(float r, float g, float b, float a)
    {
        this->r = r;
        this->b = b;
        this->g = g;
        this->a = a;
    };

    static const Color red;
    static const Color green;
    static const Color blue;
    static const Color purple;
    static const Color yellow;
    static const Color cyan;
};



namespace Graphics
{
    void ClearDrawBuffer();

    void DrawTriangle(Vector2F pos0, Vector2F pos1, Vector2F pos2, Color color);
    void DrawQuad(Vector2F pos0, Vector2F pos1, Vector2F pos2, Vector2F pos3, Color color);
    void DrawRect(Vector2F pos, float width, float height, Color color);
    void DrawLine(Vector2F startPos, Vector2F endPos, float thickness, Color color);
    void DrawCircle(Vector2F center, float radius, int numSegments, Color color);
};