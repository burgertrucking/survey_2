#ifndef TEXTDRAW_H
#define TEXTDRAW_H

#include "SDL.h"
#include "types.h"

/* struct for bitmap fonts */
typedef struct Fonts
{
    SDL_Surface* lwMain;
    SDL_Surface* dwMain;
} Fonts;

typedef enum FontType
{
    FONT_MAIN_LW,
    FONT_MAIN_DW,
} FontType;

int InitFonts(Fonts* fonts);

/* Draw a message at a given position */
int DrawText(String msg, Fonts* fontImgs, SDL_Surface* screen, FontType font, Vec2 pos);

#endif /* TEXTDRAW_H */
