#ifndef ROOM_H
#define ROOM_H

#include "SDL.h"
#include "types.h"

enum
{
    TILE_SIZE = 20,
    MAX_TILE_DIM = 64, /* square this for total amount of tiles */
    MAX_WALLS = 64,
    MAX_SLOPES = 32,
    MAX_ROOM_MESSAGES = 64,
    MAX_INTERACTABLES = 64,
    MAX_INTERACTABLE_CHECKS = 8,
};

typedef enum RoomSheetType
{
    ROOM_SHEET_TILEMAP, /* the image is the room's tileset (eg. lw school hallway) */
    ROOM_SHEET_WHOLE, /* the image is of the room (eg. lw classrooms) */
} RoomSheetType;

typedef struct Tile
{
    Vec2 sprPos; /* sprite's position in tilemap (unit: pixels) */
} Tile;

typedef struct RoomSheet
{
    SDL_Surface* img;
    RoomSheetType type;
} RoomSheet;

/* defines which corner the slope's right angle is located */
/* (hypotenuse is opposite this point) */
typedef enum SlopeType
{
    SLOPE_TOP_LEFT,
    SLOPE_TOP_RIGHT,
    SLOPE_BOTTOM_LEFT,
    SLOPE_BOTTOM_RIGHT,
} SlopeType;
typedef struct Slope
{
    Vec2 pos;
    SlopeType corner;
} Slope;

typedef struct Interactable
{
    Rect bbox; /* nonsolids have a bbox of (0, 0, 0, 0) */
    int msgs[MAX_INTERACTABLE_CHECKS];
    int msgsLen;
    int checkCount;
} Interactable;

typedef struct RoomMessage
{
    String msg;
    int next; /* -1 means textbox closes after this message */
} RoomMessage;

typedef struct Room
{
    Vec2 size; /* in tiles */
    SDL_Surface* surface; /* all tiles are drawn to this surface upon init */
    SDL_Surface* slopespr;
    RoomSheet sheet;
    RoomMessage msgs[MAX_ROOM_MESSAGES];
    Tile tiles[MAX_TILE_DIM][MAX_TILE_DIM];
    Rect walls[MAX_WALLS];
    Slope slopes[MAX_SLOPES];
    Interactable interactables[MAX_INTERACTABLES];
    int tilesLen;
    int wallsLen;
    int slopesLen;
    int interactablesLen;
    /* NOTE: length of msgs is not currently tracked */
} Room;

int InitRoom(Room* r, const char* sheetFile, RoomSheetType sheetType);
int DrawRoom(Room* r, SDL_Surface* screen);
int DrawRoomGizmos(Room* r, SDL_Surface* screen);

#endif /* ROOM_H */
