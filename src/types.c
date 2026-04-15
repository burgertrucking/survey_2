#include "types.h"

SDL_bool RectCheckCollisions(Rect a, Rect b)
{
    return
        (a.x + a.w) > b.x &&
        a.x < (b.x + b.w) &&
        (a.y + a.h) > b.y &&
        a.y < (b.y + b.h)
    ;
}

Vec2 Vec2Add(Vec2 a, Vec2 b)
{
    return (Vec2){ a.x+b.x, a.y+b.y };
}

Vec2 Vec2Subtract(Vec2 a, Vec2 b)
{
    return (Vec2){ a.x-b.x, a.y-b.y };
}

Vec2 Vec2Scale(Vec2 v, float scale)
{
    return (Vec2){ v.x*scale, v.y*scale };
}

float Vec2DotProduct(Vec2 a, Vec2 b)
{
    return a.x*b.x + a.y*b.y;
}

SDL_bool Vec2Equals(Vec2 a, Vec2 b)
{
    return (a.x == b.x && a.y == b.y);
}
