#pragma once

#include <vector>

#if !defined(RL_VECTOR2_TYPE)
// Vector2 type
typedef struct Vector2
{
    float x;
    float y;
} Vector2;
#define RL_VECTOR2_TYPE
#endif

#if !defined(RL_RECTANGLE_TYPE)
// Rectangle, 4 components
typedef struct Rectangle
{
    float x;      // Rectangle top-left corner position x
    float y;      // Rectangle top-left corner position y
    float width;  // Rectangle width
    float height; // Rectangle height
} Rectangle;
#define RL_RECTANGLE_TYPE
#endif

#if !defined(RL_COLOR_TYPE)
typedef struct Color {
    unsigned char r;        // Color red value
    unsigned char g;        // Color green value
    unsigned char b;        // Color blue value
    unsigned char a;        // Color alpha value
} Color;
#define RL_COLOR_TYPE
#endif

#ifndef PI
    #define PI 3.14159265358979323846f
#endif

#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif

#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif


// Vector2 operators ------------------------------------------------------------------------------------------

inline Vector2 operator+(const Vector2& a, const Vector2& b)
{
    return {a.x + b.x, a.y + b.y};
}

inline Vector2 operator+(const Vector2& a, const float value)
{
    return {a.x + value, a.y + value};
}

inline Vector2 operator-(const Vector2& a, const Vector2& b)
{
    return {a.x - b.x, a.y - b.y};
}

inline Vector2 operator-(const Vector2& a, const float value)
{
    return {a.x - value, a.y - value};
}

inline Vector2 operator*(const Vector2& a, const Vector2& b)
{
    return {a.x * b.x, a.y * b.y};
}

inline Vector2 operator*(const Vector2& a, const float value)
{
    return {a.x * value, a.y * value};
}

inline Vector2 operator/(const Vector2& a, const Vector2& b)
{
    return {a.x / b.x, a.y / b.y};
}

inline Vector2 operator/(const Vector2& a, const float value)
{
    return {a.x / value, a.y / value};
}

inline void operator+=(Vector2& a, const Vector2& b)
{
    a = {a.x + b.x, a.y + b.y};
}

inline void operator-=(Vector2& a, const Vector2& b)
{
    a = {a.x - b.x, a.y - b.y};
}

inline void operator*=(Vector2& a, const Vector2& b)
{
    a = {a.x * b.x, a.y * b.y};
}

inline void operator/=(Vector2& a, const Vector2& b)
{
    a = {a.x / b.x, a.y / b.y};
}

inline void operator+=(Vector2& a, const float value)
{
    a = {a.x + value, a.y + value};
}

inline void operator-=(Vector2& a, const float value)
{
    a = {a.x - value, a.y - value};
}

inline void operator*=(Vector2& a, const float value)
{
    a = {a.x * value, a.y * value};
}

inline void operator/=(Vector2& a, const float value)
{
    a = {a.x / value, a.y / value};
}

// -----------------------------------------------------------------------------------------------------------------
// Rectangle Functions ---------------------------------------------------------------------------------------------

// Get the position of the rectangle
inline Vector2 GetRectanglePosition(const Rectangle& rectangle)
{
    return { rectangle.x, rectangle.y };
}

// Get the size of the rectangle
inline Vector2 GetRectangleSize(const Rectangle& rectangle)
{
    return { rectangle.width, rectangle.height };
}

// -----------------------------------------------------------------------------------------------------------------
// Color Functions -------------------------------------------------------------------------------------------------

// Lerps from one color to another depending on progress
inline Color LerpColor(Color start, Color end, float progress)
{
    int r = start.r + progress * (end.r - start.r);
    int g = start.g + progress * (end.g - start.g);
    int b = start.b + progress * (end.b - start.b);
    int a = start.a + progress * (end.a - start.a);
    return {(unsigned char)r, (unsigned char)g, (unsigned char)b, (unsigned char)a};
}

// Lerp from each color to the next depending on progress, where 0.0f is the first color and 1.0f is the last, while going trough each color
inline Color LerpColor(std::vector<Color> colors, float progress)
{
	int n_colors = colors.size();
	int index_from = (int)(progress * (n_colors - 1));
	if (index_from > n_colors - 2)
		return colors[n_colors - 1];
	return LerpColor(colors[index_from], colors[index_from + 1], (progress - (1.0f * index_from) / (n_colors - 1)) * (n_colors - 1));
}
