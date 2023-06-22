#pragma once

#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

namespace Calcul
{
    // the ratio of the circumference to the radius of a circle, which is equal to 2π
    constexpr float TAU = 2.0f * M_PI;
};

template <class T>
struct Vector2
{
    T x, y;

    Vector2(T x, T y)
    {
        this->x = x;
        this->y = y;
    }

    static const Vector2 zero;
    static const Vector2 one;
    static const Vector2 up;
    static const Vector2 right;
    static const Vector2 left;
    static const Vector2 down;

    constexpr Vector2<T> operator+(const Vector2<T>& v) const
    {
        return Vector2<T>(this->x + v.x, this->y + v.y);
    }

    constexpr Vector2<T> operator-(const Vector2<T>& v) const
    {
        return Vector2<T>(this->x - v.x, this->y - v.y);
    }
};

template<class T> const Vector2<T> Vector2<T>::zero  = Vector2<T>(0, 0);
template<class T> const Vector2<T> Vector2<T>::one   = Vector2<T>(1, 1);
template<class T> const Vector2<T> Vector2<T>::up    = Vector2<T>(0, 1);
template<class T> const Vector2<T> Vector2<T>::right = Vector2<T>(1, 0);
template<class T> const Vector2<T> Vector2<T>::left  = Vector2<T>(-1, 0);
template<class T> const Vector2<T> Vector2<T>::down  = Vector2<T>(0, -1);

using Vector2F   = Vector2<float>;
using Vector2Int = Vector2<int>;

// Vector2F operator*(Vector2F v, float scale)
// {
//     return Vector2F { v.x * scale, v.y * scale };
// }