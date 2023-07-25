#pragma once

#include "../include/Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float vertexBuffer[1000];
int   vertexBufferUsed = 0;

uint32_t indexBuffer[1000];
int      indexBufferUsed = 0;

DrawCommand drawCommandBuffer[10];
int drawCommandCount = 0;
DrawCommand currentDrawCommand = {};

const Color Color::red     = Color(1.f, 0.f, 0.f, 1.f);
const Color Color::green   = Color(0.f, 1.f, 0.f, 1.f);
const Color Color::blue    = Color(0.f, 0.f, 1.f, 1.f);
const Color Color::purple  = Color(1.f, 0.f, 1.f, 1.f);
const Color Color::yellow  = Color(1.f, 1.f, 0.f, 1.f);
const Color Color::cyan    = Color(0.f, 1.f, 1.f, 1.f);
const Color Color::white   = Color(1.f, 1.f, 1.f, 1.f);
const Color Color::baseImg = Color::white;

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace Graphics
{
    Vector2F translateVec = Vector2F(200.f, 200.f);

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

    void Translate(Vector2F translate)
    {
        translateVec = translate;
    }

    Vector2F Transform(Vector2F vec2)
    {
        return vec2 += translateVec;
    }

    Vector2F Scale(Vector2F pos, Vector2F scaleVec2, Vector2F pivot)
    {
        Vector2F pivotTranslatedVec = pos - pivot;

        pivotTranslatedVec *= scaleVec2;

        pivotTranslatedVec += pivot;

        return pivotTranslatedVec;
    }

    void AppendVertex(Vector2F pos, Color color, float u = 0, float v = 0)
    {
        Vector2F translatedVec2 = Transform(pos);

        Vector2F scaledVec = Scale(pos, Vector2F(1.f, 1.f), Vector2F(4.f, 4.f));

        //printf("%f %f\n", translatedVec2.x, translatedVec2.y);

        Vector2F vertexPos = PosToVertex(scaledVec);

        vertexBuffer[vertexBufferUsed++] = vertexPos.x;
        vertexBuffer[vertexBufferUsed++] = vertexPos.y;
        vertexBuffer[vertexBufferUsed++] = 0.5f;
        vertexBuffer[vertexBufferUsed++] = color.r;
        vertexBuffer[vertexBufferUsed++] = color.g;
        vertexBuffer[vertexBufferUsed++] = color.b;
        vertexBuffer[vertexBufferUsed++] = color.a;
        vertexBuffer[vertexBufferUsed++] = u;
        vertexBuffer[vertexBufferUsed++] = v;
    }

    void DrawTriangle(Vector2F pos0, Vector2F pos1, Vector2F pos2, Color color)
    {
        int startIndex = vertexBufferUsed / (3 + 4 + 2); // /7 = 1 vertex.

        AppendVertex(pos0, color);
        AppendVertex(pos1, color);
        AppendVertex(pos2, color);

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 1;
        indexBuffer[indexBufferUsed++] = startIndex + 2;
    }

    void DrawQuad(Vector2F pos0, Vector2F pos1, Vector2F pos2, Vector2F pos3, Color color)
    {
        int startIndex = vertexBufferUsed / (3 + 4 + 2); // /7 = 1 vertex.

        AppendVertex(pos0, color, 0, 0);
        AppendVertex(pos1, color, 1, 0);
        AppendVertex(pos2, color, 1, 1);
        AppendVertex(pos3, color, 0, 1);

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 1;
        indexBuffer[indexBufferUsed++] = startIndex + 2;

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 2;
        indexBuffer[indexBufferUsed++] = startIndex + 3;
    }

    void DrawRect(Vector2F pos, float width, float height, Color color, sg_image* texture)
    {
        if (!currentDrawCommand.texture || texture->id != currentDrawCommand.texture->id)
        {
            DrawCommand cmd = (DrawCommand){ .texture = texture, .indexStart = indexBufferUsed, .indexCount = 6 };
            currentDrawCommand = cmd;
            drawCommandBuffer[drawCommandCount++] = cmd;
        }
        else 
        {
            DrawCommand* lastCommand = &drawCommandBuffer[drawCommandCount - 1];
            lastCommand->indexCount += 6;
        }

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
        int startIndex = vertexBufferUsed / 9; // 7 = 3 (position) + 4 (color) attributes per vertex

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

    Bitmap LoadImg(const char* filePath)
    {
        Bitmap img = {};

        int dummyChannels; // dummy (not used), only here to be called in stbi_load because we always want 4 channels so we hard code it later.

        img.pixels = (uint32_t*) stbi_load(filePath, &img.pixelSizeX, &img.pixelSizeY, &dummyChannels, 4);
        img.channels = 4;

        if(img.pixels == nullptr) 
        {
            printf("Error in loading the image %s \n", filePath);
            exit(1);
        }

        printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", img.pixelSizeX, img.pixelSizeY, img.channels);

        return img;
    }

    void DrawTile(Vector2F pos, float width, float height, Color color, float uMin, float uMax, float vMin, float vMax)
    {
        int startIndex = vertexBufferUsed / (3 + 4 + 2); // /7 = 1 vertex.

        AppendVertex(pos, color, uMin, vMin);
        AppendVertex(Vector2F(pos.x + width, pos.y),          color, uMax, vMin);
        AppendVertex(Vector2F(pos.x + width, pos.y + height), color, uMax, vMax);
        AppendVertex(Vector2F(pos.x, pos.y + height),         color, uMin, vMax);

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 1;
        indexBuffer[indexBufferUsed++] = startIndex + 2;

        indexBuffer[indexBufferUsed++] = startIndex + 0;
        indexBuffer[indexBufferUsed++] = startIndex + 2;
        indexBuffer[indexBufferUsed++] = startIndex + 3;
    }
}