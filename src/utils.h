#ifndef UTILS_H
#define UTILS_H

#include "SDL.h"
#include "types.h"

/* Load a png image and save it to an SDL_Surface with the same format as the screen */
SDL_Surface* LoadImage(const char* file);
/* Convenience wrapper for blitting a surface at a given point */
int BlitSurfaceCoords(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, Vec2 pos);
/* Scale a surface then blit it. Does not alpha blend */
int BlitSurfaceScaled(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, Vec2 pos, Vec2 scale);
/* Check if two line segments (defined by their endpoints) intersect */
/* NOTE: May fit better in types.c */
SDL_bool LineCheckCollisions(Vec2 a1, Vec2 b1, Vec2 a2, Vec2 b2);

#endif
