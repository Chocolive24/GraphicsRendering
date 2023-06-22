#pragma once

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

const Color Color::red    = Color(1.f, 0.f, 0.f, 1.f);
const Color Color::green  = Color(0.f, 1.f, 0.f, 1.f);
const Color Color::blue   = Color(0.f, 0.f, 1.f, 1.f);
const Color Color::purple = Color(1.f, 0.f, 1.f, 1.f);
const Color Color::yellow = Color(1.f, 1.f, 0.f, 1.f);
const Color Color::cyan   = Color(0.f, 1.f, 1.f, 1.f);