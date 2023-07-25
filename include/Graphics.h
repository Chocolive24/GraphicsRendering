#pragma once 

#include "../include/Math.h"

#include <stdint.h>

struct sg_image;

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
    static const Color white;
    static const Color baseImg;
};

struct Vertex
{
    Vector2F position;
    Color color;
    float u, v;
};

struct Bitmap 
{
  int pixelSizeX;
  int pixelSizeY;
  int channels;
  uint32_t* pixels;

  size_t DataSize() { return static_cast<size_t>(pixelSizeX * pixelSizeY * channels); };
};

struct DrawCommand
{   
    sg_image* texture; // pointer only because of the sokol link problem.
    int indexStart;
    int indexCount;
};

namespace Graphics
{
    extern Vector2F translateVec;

    void ClearDrawBuffer();

    Vector2F PosToVertex(Vector2F pos);

    void Translate(Vector2F translate);

    Vector2F Transform(Vector2F vec2);
    Vector2F Scale(Vector2F scaleVec2, Vector2F pivot);

    void DrawTriangle(Vector2F pos0, Vector2F pos1, Vector2F pos2, Color color);
    void DrawQuad(Vector2F pos0, Vector2F pos1, Vector2F pos2, Vector2F pos3, Color color);
    void DrawRect(Vector2F pos, float width, float height, Color color, sg_image* texture);
    void DrawLine(Vector2F startPos, Vector2F endPos, float thickness, Color color);
    void DrawCircle(Vector2F center, float radius, int numSegments, Color color);

    Bitmap LoadImg(const char* filePath);

    void DrawTile(Vector2F pos, float width, float height, Color color, float uMin, float uMax, float vMin, float vMax);
};