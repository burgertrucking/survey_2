#ifndef TYPES_H
#define TYPES_H

#include "SDL.h"

/* Distinct from SDL_Rect in that its fields are floats */
typedef struct Rect
{
    float x;
    float y;
    float w;
    float h;
} Rect;

/* Rect functions */
SDL_bool RectCheckCollisions(Rect a, Rect b);

typedef struct Vec2
{
    float x;
    float y;
} Vec2;

/* Vec2 functions */
Vec2 Vec2Add(Vec2 a, Vec2 b);
Vec2 Vec2Subtract(Vec2 a, Vec2 b);
Vec2 Vec2Scale(Vec2 v, float scale);
float Vec2DotProduct(Vec2 a, Vec2 b);
SDL_bool Vec2Equals(Vec2 a, Vec2 b);

/* string type with length */
/* NOTE: Consider adding a constructor function (eg. automatically calculates len) */
typedef struct String
{
    char* data;
    unsigned int len;
} String;

/* TEMP macro versions */
/*
#define Vec2Add(A, B) (Vec2){ A.x+B.x, A.y+B.y }
#define Vec2Subtract(A, B) (Vec2){ A.x-B.x, A.y-B.y }
#define Vec2Scale(V, S) (Vec2){ V.x*S, V.y*S }
#define Vec2DotProduct(A, B) (float)(A.x*B.x + A.y*B.y)
#define Vec2Equals(A, B) (A.x == B.x && A.y == B.y)
*/

#endif /* TYPES_H */
