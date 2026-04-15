#ifndef TEXTBOX_H
#define TEXTBOX_H

#include "SDL.h"
#include "textdraw.h"
#include "room.h"

typedef struct Textbox
{
    SDL_Surface* graphic;
    unsigned int charsDrawn;
    SDL_bool shouldDraw;
    int msgToDraw; /* value of -1 indicates an erroneous message index */
} Textbox;

int InitTextbox(Textbox* tb);
void UpdateTextbox(Textbox* tb, RoomMessage* msgs, Uint32 vPad, Uint32* status);
/* expects vscreen480 */
int DrawTextbox(Textbox* tb, RoomMessage* msgs, SDL_bool isDW, Fonts* fnt, SDL_Surface* screen);

#endif /* TEXTBOX_H */
