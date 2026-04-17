#include "textdraw.h"
#include <stdio.h>
#include "utils.h"

enum
{
    TEXT_CHAR_WIDTH = 17,
    TEXT_CHAR_HEIGHT = 33,

    FONT_IMG_ROWS = 9,
    FONT_IMG_COLS = 30,

    TEXT_CHAR_SPACING_DEFAULT = TEXT_CHAR_WIDTH - 1,
    TEXT_LINE_SPACING_DEFAULT = TEXT_CHAR_HEIGHT + 3
};

/* Calculate the rectangle that clips a given character from the bitmap font */
static SDL_Rect GetCharRect(const char letter);

int InitFonts(Fonts* fonts)
{
    fonts->lwMain = LoadImage("res/rip/fnt/maintext-lw.png");
    fonts->dwMain = LoadImage("res/rip/fnt/maintext-dw.png");
    if (!fonts->lwMain || !fonts->dwMain)
    {
        fprintf(stderr, "InitFonts: Could not load bitmap font files\n");
        return 1;
    }
    return 0;
}

int DrawText(String msg, Fonts* fontImgs, SDL_Surface* screen, FontType font, Vec2 pos)
{
    const int startX = pos.x - TEXT_CHAR_SPACING_DEFAULT;
    SDL_Surface* f;
    Vec2 charPos;
    unsigned int i;

    switch (font)
    {
        case FONT_MAIN_LW:
            f = fontImgs->lwMain;
        break;

        case FONT_MAIN_DW:
            f = fontImgs->dwMain;
        break;

        default:
            printf("WARNING: DrawText: Invalid fonttype %i specified, returning early\n", font);
            return 1;
        break;
    }

    /* null character check allows for rendering strings shorter than their buffer size (eg. when drawing fps) */
    /* TODO log when null character check goes off */
    for (i = 0, charPos.x = startX, charPos.y = pos.y; i < msg.len && msg.data[i] != '\0'; ++i)
    {
        if (msg.data[i] != '\n')
        {
            SDL_Rect srcrect = GetCharRect(msg.data[i]);
            charPos.x += TEXT_CHAR_SPACING_DEFAULT;
            BlitSurfaceCoords(f, &srcrect, screen, charPos);
        }
        else
        {
            charPos.x = startX;
            charPos.y += TEXT_LINE_SPACING_DEFAULT;
        }
    }

    return 0;
}

static SDL_Rect GetCharRect(const char letter)
{
    int col = letter%FONT_IMG_COLS;
    int row = (letter - col)/FONT_IMG_COLS;
    return NewSDL_Rect(col*TEXT_CHAR_WIDTH, row*TEXT_CHAR_HEIGHT, TEXT_CHAR_WIDTH, TEXT_CHAR_HEIGHT);
}
