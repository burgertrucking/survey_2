#include "types.h"
#include "string.h"

Rect NewRect(float x, float y, float w, float h)
{
    Rect result;
    result.x = x; result.y = y; result.w = w; result.h = h;
    return result;
}

SDL_bool RectCheckCollisions(Rect a, Rect b)
{
    return
        (a.x + a.w) > b.x &&
        a.x < (b.x + b.w) &&
        (a.y + a.h) > b.y &&
        a.y < (b.y + b.h)
    ;
}

Vec2 NewVec2(float x, float y)
{
    Vec2 result;
    result.x = x; result.y = y;
    return result;
}

Vec2 Vec2Zero(void)
{
    Vec2 result;
    result.x = 0; result.y = 0;
    return result;
}

Vec2 Vec2Add(Vec2 a, Vec2 b)
{
    Vec2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Vec2 Vec2Subtract(Vec2 a, Vec2 b)
{
    Vec2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

Vec2 Vec2Scale(Vec2 v, float scale)
{
    Vec2 result;
    result.x = v.x*scale;
    result.y = v.y*scale;
    return result;
}

float Vec2DotProduct(Vec2 a, Vec2 b)
{
    return a.x*b.x + a.y*b.y;
}

SDL_bool Vec2Equals(Vec2 a, Vec2 b)
{
    return (a.x == b.x && a.y == b.y);
}

String NewString(char* data)
{
    String result;
    result.data = data;
    result.len = strlen(data);
    return result;
}
