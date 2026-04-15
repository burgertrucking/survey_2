#ifndef PLAYER_H
#define PLAYER_H

#include "SDL.h"
#include "types.h"
#include "room.h"
#include "textbox.h"

typedef struct Player {
	SDL_Surface* lwSprite;
	SDL_Surface* dwSprite;
	Rect bbox;
    Rect checkBbox;
	Vec2 pos;
    unsigned int runCount; /* counts frames moved for acceleration */
	unsigned int stillCount; /* counts consecutive frames player stands still; used to play animations while tap moving (not exact to original) */
    unsigned int frameCount;
    unsigned int animFrame;
    unsigned int facing;
	 /* TODO move to a bitflag with other bools */
    SDL_bool isRunning;
    SDL_bool noclip; /* toggled in game.c with debug enabled mode */
} Player;

int InitPlayer(Player* p);
void UpdatePlayer(Player* p, Room* room, Textbox* tb, Uint32 vPad, Uint32* status);
int DrawPlayer(Player* p, SDL_Surface* screen, Uint32 status);
/* Should be drawn into vscreen240 */
int DrawPlayerGizmos(Player* p, SDL_Surface* screen);

#endif /* PLAYER_H */
