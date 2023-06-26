#pragma once

#include <stdint.h> // for uint32_t
#include <stdio.h> // for printf
#include <stdlib.h> // for exit

#include "../include/Graphics.h"

float vertexBuffer[1000];
int   vertexBufferUsed = 0;

uint32_t indexBuffer[1000];
int      indexBufferUsed = 0;

const Color Color::red    = Color(1.f, 0.f, 0.f, 1.f);
const Color Color::green  = Color(0.f, 1.f, 0.f, 1.f);
const Color Color::blue   = Color(0.f, 0.f, 1.f, 1.f);
const Color Color::purple = Color(1.f, 0.f, 1.f, 1.f);
const Color Color::yellow = Color(1.f, 1.f, 0.f, 1.f);
const Color Color::cyan   = Color(0.f, 1.f, 1.f, 1.f);

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace Graphics
{
    void ClearDrawBuffer()
    {
        vertexBufferUsed = 0;
        indexBufferUsed = 0;
    }

    Vector2F PosToVertex(Vector2F pos)
    {
        Vector2F vertexPos;

        vertexPos.x =   pos.x * 2 / sapp_width()  - 1;
        vertexPos.y = -(pos.y * 2 / sapp_height() - 1);

        return vertexPos;
    }

    void AppendVertex(Vector2F pos, Color color)
    {
        Vector2F vertexPos = PosToVertex(pos);

        vertexBuffer[vertexBufferUsed++] = vertexPos.x;
        vertexBuffer[vertexBufferUsed++] = vertexPos.y;
        vertexBuffer[vertexBufferUsed++] = 0.5f;
        vertexBuffer[vertexBufferUsed++] = color.r;
        vertexBuffer[vertexBufferUsed++] = color.g;
        vertexBuffer[vertexBufferUsed++] = color.b;
        vertexBuffer[vertexBufferUsed++] = color.a;
    }

    void DrawTriangle(Vector2F pos0, Vector2F pos1, Vector2F pos2, Color color)
    {
        int startIndex = vertexBufferUsed / (3 + 4); // /7 = 1 vertex.

        AppendVertex(pos0, color);
        AppendVertex(pos1, color);
        AppendVertex(pos2, color);

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 1;
        indexBuffer[indexBufferUsed++] = startIndex + 2;
    }

    void DrawQuad(Vector2F pos0, Vector2F pos1, Vector2F pos2, Vector2F pos3, Color color)
    {
        int startIndex = vertexBufferUsed / (3 + 4); // /7 = 1 vertex.

        AppendVertex(pos0, color);
        AppendVertex(pos1, color);
        AppendVertex(pos2, color);
        AppendVertex(pos3, color);

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 1;
        indexBuffer[indexBufferUsed++] = startIndex + 2;

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 2;
        indexBuffer[indexBufferUsed++] = startIndex + 3;
    }

    void DrawRect(Vector2F pos, float width, float height, Color color)
    {
        DrawQuad(pos, Vector2F(pos.x + width, pos.y), Vector2F(pos.x + width, pos.y + height), 
                 Vector2F(pos.x, pos.y + height), color);
    }

    void DrawLine(Vector2F startPos, Vector2F endPos, float thickness, Color color)
    {
        if (startPos == endPos) return;

        Vector2F vSE = endPos - startPos;

        Vector2F normalized = vSE.Normalized();
        Vector2F perp(normalized.y, -(normalized.x));

        Vector2F pos0 = startPos + perp * (thickness / 2.f);
        Vector2F pos1 = endPos   + perp * (thickness / 2.f);
        Vector2F pos2 = endPos   - perp * (thickness / 2.f);
        Vector2F pos3 = startPos - perp * (thickness / 2.f);

        DrawQuad(pos0, pos1, pos2, pos3, color);
    }

    void DrawCircle(Vector2F center, float radius, int numSegments, Color color)
    {
        int startIndex = vertexBufferUsed / 7; // 7 = 3 (position) + 4 (color) attributes per vertex

        const float angleStep = Calcul::TAU / numSegments;

        for (int i = 0; i < numSegments; i++)
        {
            float angle = i * angleStep;
            float x = center.x + radius * cos(angle);
            float y = center.y + radius * sin(angle);
            AppendVertex(Vector2F(x, y), color);
        }

        // Connect the last vertex with the first vertex to close the circle
        AppendVertex(Vector2F(center.x + radius, center.y), color);

        for (int i = 0; i < numSegments; i++)
        {
            indexBuffer[indexBufferUsed++] = startIndex + i;
            indexBuffer[indexBufferUsed++] = startIndex + i + 1;
            indexBuffer[indexBufferUsed++] = startIndex + numSegments;
        }
    }
}