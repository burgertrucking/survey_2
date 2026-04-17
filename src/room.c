#include "room.h"
#include "utils.h"

int InitRoom(Room* r, const char* sheetFile, RoomSheetType sheetType)
{
    int err = 0;

    r->sheet.img = LoadImage(sheetFile);
    r->slopespr = LoadImage("res-temp/corner-tiles.png");
    r->sheet.type = sheetType;
    if (r->sheet.type == ROOM_SHEET_WHOLE)
    {
        r->surface = r->sheet.img;
        r->tilesLen = 0;
    }
    else if (!r->sheet.img || !r->slopespr)
    {
        err = 1;
    }
    else
    {
        printf("WARNING: InitRoom() sheet type tilemap currently unhandled\n");
        err = 2;
        return err;
    }

    /* TODO handle creating walls and interactables */

    return err;
}

/* can either draw to vscreen240 or vscreen480 depending on room specifics */
int DrawRoom(Room* r, SDL_Surface* screen)
{
    return BlitSurfaceCoords(r->surface, NULL, screen, Vec2Zero());
}

int DrawRoomGizmos(Room* r, SDL_Surface* screen)
{
    int i, err = 0;
    for (i = 0; i < r->wallsLen; ++i)
    {
        /* TEMP draw solid blue rectangles the size of each wall */
        SDL_Rect bboxGfx = NewSDL_Rect(r->walls[i].x, r->walls[i].y,
                                       r->walls[i].w, r->walls[i].h);
        err = SDL_FillRect(screen, &bboxGfx, SDL_MapRGB(screen->format, 0, 0, 255));
        if (err) return err;
    }
    for (i = 0; i < r->interactablesLen; ++i)
    {
        /* TEMP draw solid pink rectangles the size of each interactable */
        SDL_Rect bboxGfx = NewSDL_Rect(
            r->interactables[i].bbox.x, r->interactables[i].bbox.y,
            r->interactables[i].bbox.w, r->interactables[i].bbox.h
        );
        err = SDL_FillRect(screen, &bboxGfx, SDL_MapRGB(screen->format, 255, 128, 128));
        if (err) return err;
    }
    for (i = 0; i < r->slopesLen; ++i)
    {
        SDL_Rect ssprRect = NewSDL_Rect(r->slopes[i].corner*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
        err = BlitSurfaceCoords(r->slopespr, &ssprRect, screen, r->slopes[i].pos);
        if (err) return err;
    }

    return err;
}
