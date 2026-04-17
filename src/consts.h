#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "SDL.h"

/* anonymous enum for constants */
enum
{
    RES_WIDTH = 640,
    RES_HEIGHT = 480,
    WORLD_RES_WIDTH = 320,
    WORLD_RES_HEIGHT = 240,
    SCREEN_BPP = 0, /* native colour depth */
    TICKS_PER_SECOND = 30,
    DEFAULT_VIDEO_FLAGS = SDL_SWSURFACE|SDL_ANYFORMAT, /* NOTE may eventually let machine decide hw vs sw rather than hardcoding */
    WINDOW_RESIZABLE = SDL_RESIZABLE|DEFAULT_VIDEO_FLAGS,
    WINDOW_FULLSCREEN = SDL_FULLSCREEN|DEFAULT_VIDEO_FLAGS
};

#endif /* CONSTANTS_H */
