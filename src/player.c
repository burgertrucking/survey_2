#include "player.h"
#include <stdio.h>
#include "utils.h"
#include "input.h"
#include "bitflag.h"
#include "statusbf.h"
#include "consts.h"

/* constants */
enum
{
    PLAYER_SPRITE_WIDTH = 19,
    PLAYER_SPRITE_HEIGHT = 38,

    PLAYER_BBOX_WIDTH = PLAYER_SPRITE_WIDTH,
    PLAYER_BBOX_HEIGHT = 14,
    PLAYER_BBOX_Y_OFFSET = 25,

    PLAYER_CHK_BBOX_X_OFFSET_VER = 4,
    PLAYER_CHK_BBOX_X_OFFSET_RIGHT = PLAYER_SPRITE_WIDTH/2,
    PLAYER_CHK_BBOX_X_OFFSET_LEFT = -15,
    PLAYER_CHK_BBOX_Y_OFFSET_HOR = PLAYER_BBOX_Y_OFFSET,
    PLAYER_CHK_BBOX_Y_OFFSET_DOWN = 28,
    PLAYER_CHK_BBOX_Y_OFFSET_UP = 5,
    PLAYER_CHK_BBOX_WIDTH_VER = PLAYER_SPRITE_WIDTH - 8,
    PLAYER_CHK_BBOX_HEIGHT_VER = PLAYER_SPRITE_HEIGHT - 11,
    PLAYER_CHK_BBOX_WIDTH_HOR = PLAYER_SPRITE_WIDTH/2 + 15,
    PLAYER_CHK_BBOX_HEIGHT_HOR = PLAYER_BBOX_HEIGHT,

    PLAYER_FACE_DOWN = 0,
    PLAYER_FACE_RIGHT,
    PLAYER_FACE_UP,
    PLAYER_FACE_LEFT,

    PLAYER_WALK_SPEED_BASE = 2,
    PLAYER_WALK_SPEED_LW = PLAYER_WALK_SPEED_BASE + 1,
    PLAYER_WALK_SPEED_DW = PLAYER_WALK_SPEED_BASE,
    PLAYER_RUN_START_SPEED_LW = PLAYER_WALK_SPEED_BASE + 2,
    PLAYER_RUN_SPEED_LW = PLAYER_WALK_SPEED_BASE + 3,
    PLAYER_RUN_LONG_SPEED_LW = PLAYER_WALK_SPEED_BASE + 4,
    PLAYER_RUN_START_SPEED_DW = PLAYER_WALK_SPEED_BASE + 1,
    PLAYER_RUN_SPEED_DW = PLAYER_WALK_SPEED_BASE + 2,
    PLAYER_RUN_LONG_SPEED_DW = PLAYER_WALK_SPEED_BASE + 3,

    PLAYER_WALK_FPS = 4,
    PLAYER_RUN_FPS = 8
};

static Rect calcCheckBbox(Player* p);

int InitPlayer(Player* p)
{
    p->lwSprite = LoadImage("res/edit/spr/mainchara-lw.png");
    p->dwSprite = LoadImage("res/rip/spr/mainchara-dw.png");
    if (!p->lwSprite || !p->dwSprite)
    {
        fprintf(stderr, "InitPlayer: Could not load sprites\n");
        return 1;
    }
    p->animFrame = 0;
    p->facing = PLAYER_FACE_DOWN;
    p->pos = NewVec2(260, 110); /* TEMP these are hardcoded for the classroom */
    p->bbox = NewRect(p->pos.x, p->pos.y + PLAYER_BBOX_Y_OFFSET, PLAYER_BBOX_WIDTH, PLAYER_BBOX_HEIGHT);
    p->checkBbox = calcCheckBbox(p);
    p->runCount = 0;
    return 0;
}

void UpdatePlayer(Player* p, Room* room, Textbox* tb, Uint32 vPad, Uint32* status)
{
	SDL_bool isCutscene, isDarkWorld, isMoving;
	int moveSpeed;
	Vec2 dir, dp, newPos;
	int i;

	isCutscene = CheckFlag(*status, STATUS_IS_CUTSCENE);
	isDarkWorld = CheckFlag(*status, STATUS_IS_DARK_WORLD);
	/* STUB just don't update player when in cutscene */
	/* consider using fsm instead for this */
	if (isCutscene) return;

    /* handle inputs */
    p->isRunning = CheckVInput(vPad, VKEY_CANCEL_HELD);
    if (p->isRunning)
    {
    	if (p->runCount < 10) moveSpeed = (isDarkWorld)? PLAYER_RUN_START_SPEED_DW : PLAYER_RUN_START_SPEED_LW;
    	else if (p->runCount > 60) moveSpeed = (isDarkWorld)? PLAYER_RUN_LONG_SPEED_DW : PLAYER_RUN_LONG_SPEED_LW;
    	else moveSpeed = (isDarkWorld)? PLAYER_RUN_SPEED_DW : PLAYER_RUN_SPEED_LW;
    }
    else moveSpeed = (isDarkWorld)? PLAYER_WALK_SPEED_DW : PLAYER_WALK_SPEED_LW;
    dir = Vec2Zero();
    if (CheckVInput(vPad, VKEY_DOWN_HELD)) dir.y = 1;
    if (CheckVInput(vPad, VKEY_UP_HELD)) dir.y = -1;
    if (CheckVInput(vPad, VKEY_RIGHT_HELD)) dir.x = 1;
    if (CheckVInput(vPad, VKEY_LEFT_HELD)) dir.x = -1;

    /* handle movement */
    isMoving = SDL_FALSE;
    dp = Vec2Scale(dir, moveSpeed);
    newPos = Vec2Add(p->pos, dp);
    if (!p->noclip)
    {
	    Rect newBbox = NewRect(newPos.x, newPos.y + PLAYER_BBOX_Y_OFFSET, PLAYER_BBOX_WIDTH, PLAYER_BBOX_HEIGHT);
	    /* split bboxes into xy components for granularised collision detection */
	    Rect newBboxX = NewRect(newPos.x, p->pos.y + PLAYER_BBOX_Y_OFFSET, PLAYER_BBOX_WIDTH, PLAYER_BBOX_HEIGHT);
	    Rect newBboxY = NewRect(p->pos.x, newPos.y + PLAYER_BBOX_Y_OFFSET, PLAYER_BBOX_WIDTH, PLAYER_BBOX_HEIGHT);
	    for (i = 0; i < room->wallsLen; ++i)
	    {
			SDL_bool hitX = RectCheckCollisions(newBboxX, room->walls[i]);
			SDL_bool hitY = RectCheckCollisions(newBboxY, room->walls[i]);
			if (hitX || hitY)
			{
				if (hitX)
			    {
				if (dir.x < 0) newPos.x = room->walls[i].x + room->walls[i].w;
				else if (dir.x > 0) newPos.x = room->walls[i].x - p->bbox.w;
			    }
			    if (hitY)
				{
					if (dir.y < 0) newPos.y = room->walls[i].y + room->walls[i].h - PLAYER_BBOX_Y_OFFSET;
					else if (dir.y > 0) newPos.y = room->walls[i].y - p->bbox.h - PLAYER_BBOX_Y_OFFSET;
			    }
			}
			/* edge case for corner collisions that aren't detected when axes are split */
			/* unused because corner teleportation is fun */
			/*
		    else
		    {
				if (RectCheckCollisions(newBbox, room->walls[i]))
			    {
					switch (p->facing)
					{
						case PLAYER_FACE_DOWN:
						case PLAYER_FACE_UP:
							newPos.x = p->pos.x;
						break;
						case PLAYER_FACE_RIGHT:
						case PLAYER_FACE_LEFT:
							newPos.y = p->pos.y;
						break;
					}
				}
			}
			*/
	    }
	    for (i = 0; i < room->slopesLen; ++i)
	    {
	    	Slope s = room->slopes[i];
	    	/* NOTE unsure if this bbox should be baked directly into the slope struct or calculated at runtime here */
	    	Rect sloperect = NewRect(s.pos.x, s.pos.y, TILE_SIZE, TILE_SIZE);
	    	/* TEMP collision checking */
	    	/* TODO move to its own function (in room.c or here?) */
	    	if (RectCheckCollisions(newBbox, sloperect))
	    	{
	    		Vec2 hypA, hypB, rightA, rightB, bottomA, bottomB;
	    		SDL_bool rightCollided, bottomCollided;

	    		printf("UpdatePlayer: detected bbox is in slope tile at <%.2f, %.2f>, with slope %i (type %i)\n", s.pos.x, s.pos.y, i, s.corner);
	    		switch (room->slopes[i].corner)
	    		{
	    			case SLOPE_TOP_LEFT:
	    			case SLOPE_BOTTOM_RIGHT:
	    				hypA.x = s.pos.x;
	    				hypA.y = s.pos.y + TILE_SIZE;
	    				hypB.x = s.pos.x + TILE_SIZE;
	    				hypB.y = s.pos.y;
	    			break;
	    			case SLOPE_BOTTOM_LEFT:
	    			case SLOPE_TOP_RIGHT:
	    				hypA.x = s.pos.x;
	    				hypA.y = s.pos.y;
	    				hypB.x = s.pos.x + TILE_SIZE;
	    				hypB.y = s.pos.y + TILE_SIZE;
    				break;
	    		}
	    		/* TEMP currently hardcoed to handling bottom right collisions only */
	    		/* (player bbox's right and bottom sides) */
	    		rightA = NewVec2(newBbox.x + newBbox.w, newBbox.y);
	    		rightB = NewVec2(newBbox.x + newBbox.w, newBbox.y + newBbox.h);
	    		bottomA = NewVec2(newBbox.x, newBbox.y + newBbox.h);
	    		bottomB = NewVec2(newBbox.x + newBbox.w, newBbox.y + newBbox.h);
	    		rightCollided = LineCheckCollisions(hypA, hypB, rightA, rightB);
	    		bottomCollided = LineCheckCollisions(hypA, hypB, bottomA, bottomB);
	    		if (rightCollided && bottomCollided)
	    		{
	    			/* TODO move to its own function, this is general enough to be used for the rectangle walls */

	    			/* FIXME imperfect, when sliding along the slope player eventually ends up inside it at the end */

	    			/* formula from https://youtu.be/oom6R-M2lvQ */
	    			/* newdp = dp - N * (dp DOT n) */
	    			/* or: newdp = Vec2Subtract(dp, Vec2Scale(N, Vec2DotProduct(dp, N))) */

	    			if (p->facing == PLAYER_FACE_RIGHT || p->facing == PLAYER_FACE_DOWN) /* sliding isn't calculated when not facing slope */
	    			{
	    				Vec2 N, newdp, newdpScaled;

		    			N = NewVec2(-0.7071, -0.7071); /* normalised vector in northwest direction */
		    			newdp = Vec2Subtract(dp, Vec2Scale(N, Vec2DotProduct(dp, N)));
		    			printf("bbox inside aforementioned triangle\n");
		    			printf("dp = <%.2f, %.2f>\nnewdp = <%.2f, %.2f>\n", dp.x, dp.y, newdp.x, newdp.y);
		    			newdpScaled = Vec2Scale(newdp, 2); /* original game has you move faster on slopes */
		    			newPos = Vec2Add(p->pos, newdpScaled);
	    			}
	    		}
	    	}
	    }
    }
	if (!Vec2Equals(p->pos, newPos))
	{
		p->pos = newPos;
		isMoving = SDL_TRUE;
	}

    /* handle turning */
	switch (p->facing)
	{
		case PLAYER_FACE_DOWN:
			if (dir.y < 0) p->facing = PLAYER_FACE_UP;
			else if (dir.y == 0)
			{
				if (dir.x < 0) p->facing = PLAYER_FACE_LEFT;
				else if (dir.x > 0) p->facing = PLAYER_FACE_RIGHT;
			}
		break;
		case PLAYER_FACE_UP:
			if (dir.y > 0) p->facing = PLAYER_FACE_DOWN;
			else if (dir.y == 0)
			{
				if (dir.x < 0) p->facing = PLAYER_FACE_LEFT;
				else if (dir.x > 0) p->facing = PLAYER_FACE_RIGHT;
			}
		break;
		case PLAYER_FACE_RIGHT:
			if (dir.x < 0) p->facing = PLAYER_FACE_LEFT;
			else if (dir.x == 0)
			{
				if (dir.y < 0) p->facing = PLAYER_FACE_UP;
				else if (dir.y > 0) p->facing = PLAYER_FACE_DOWN;
			}
    	break;
		case PLAYER_FACE_LEFT:
			if (dir.x > 0) p->facing = PLAYER_FACE_RIGHT;
			else if (dir.x == 0)
			{
				if (dir.y < 0) p->facing = PLAYER_FACE_UP;
				else if (dir.y > 0) p->facing = PLAYER_FACE_DOWN;
			}
		break;
	}

	/* handle movement of bbox */
    p->bbox = NewRect(p->pos.x, p->pos.y + PLAYER_BBOX_Y_OFFSET, PLAYER_BBOX_WIDTH, PLAYER_BBOX_HEIGHT);

    /* handle movement of check bbox */
    p->checkBbox = calcCheckBbox(p);
    /* handle checking */
    for (i = 0; i < room->interactablesLen; ++i)
    {
        Interactable* obj = &room->interactables[i];
        if (RectCheckCollisions(p->checkBbox, obj->bbox))
        {
			if (CheckVInput(vPad, VKEY_ACCEPT))
	        {
	        	SetFlag(status, STATUS_IS_CUTSCENE);
	        	p->animFrame = 0;
	        	tb->shouldDraw = SDL_TRUE;
	        	if (obj->msgsLen == 0)
	        	{
	        		printf("WARNING: UpdatePlayer: Interactable %i has no messages, falling back to default\n", i);
	        		tb->msgToDraw = -1;
	        	}
	        	else
	        	{
	        		SDL_bool maxChecks;

	                tb->msgToDraw = obj->msgs[obj->checkCount];
	                maxChecks = obj->checkCount >= obj->msgsLen - 1;
	                if (maxChecks) printf("DEBUG: UpdatePlayer: max checks (%i) on interactable %i\n", obj->msgsLen, i);
	                else ++obj->checkCount;
	        	}
	        	isMoving = SDL_FALSE;
	        }
	    }
    }

	/* handle animations */
	/* TODO turn some of these magic numbers into named constants */
    if (isMoving)
    {
        int animFps = p->isRunning? PLAYER_RUN_FPS : PLAYER_WALK_FPS;
        ++p->frameCount;
        /* HACK using TICKS_PER_SECOND without proper import from game.c, import properly */
        if (p->frameCount >= TICKS_PER_SECOND/animFps)
        {
            p->frameCount = 0;
            ++p->animFrame;
            if (p->animFrame >= 4) p->animFrame = 0;
        }
        p->stillCount = 0;
        if (p->isRunning) ++p->runCount;
        else p->runCount = 0;
    }
    else if (p->stillCount >= 4)
    {
        p->animFrame = 0;
        p->frameCount = 0;
        p->stillCount = 0;
    }
    else
    {
		++p->stillCount;
    	p->runCount = 0;
    }
}

int DrawPlayer(Player* p, SDL_Surface* screen, Uint32 status)
{
	SDL_bool isDarkWorld;
	SDL_Rect srcRect;
	SDL_Surface* sprite;
	int err;

	isDarkWorld = CheckFlag(status, STATUS_IS_DARK_WORLD);
    srcRect = NewSDL_Rect(
        p->animFrame*PLAYER_SPRITE_WIDTH, p->facing*PLAYER_SPRITE_HEIGHT,
        PLAYER_SPRITE_WIDTH, PLAYER_SPRITE_HEIGHT
    );
    /* TEMP this probably should not be reassigned each frame */
    sprite = (isDarkWorld)? p->dwSprite : p->lwSprite;
    err = BlitSurfaceCoords(sprite, &srcRect, screen, p->pos);
    return err;
}

int DrawPlayerGizmos(Player* p, SDL_Surface* screen)
{
	SDL_Rect bboxGfx, checkGfx;
	Uint32 bboxColour;
	int err;

    /* TEMP creating rectangle of bbox */
    bboxGfx = NewSDL_Rect(p->bbox.x, p->bbox.y, p->bbox.w, p->bbox.h);
    checkGfx = NewSDL_Rect(p->checkBbox.x, p->checkBbox.y, p->checkBbox.w, p->checkBbox.h);
    /* bbox is grey if noclip, green if collisions enabled */
	bboxColour = (p->noclip)?  SDL_MapRGB(screen->format, 220, 220, 220) : SDL_MapRGB(screen->format, 0, 255, 0);
    err = SDL_FillRect(screen, &bboxGfx, bboxColour);
    err = SDL_FillRect(screen, &checkGfx, SDL_MapRGB(screen->format, 0, 255, 255));
    return err;
}

static Rect calcCheckBbox(Player* p)
{
    Rect c;
    switch (p->facing)
    {
        case PLAYER_FACE_DOWN:
            c.x = p->pos.x + PLAYER_CHK_BBOX_X_OFFSET_VER;
            c.y = p->pos.y + PLAYER_CHK_BBOX_Y_OFFSET_DOWN;
            c.w = PLAYER_CHK_BBOX_WIDTH_VER;
            c.h = PLAYER_CHK_BBOX_HEIGHT_VER;
        break;
        case PLAYER_FACE_RIGHT:
            c.x = p->pos.x + PLAYER_CHK_BBOX_X_OFFSET_RIGHT;
            c.y = p->pos.y + PLAYER_CHK_BBOX_Y_OFFSET_HOR;
            c.w = PLAYER_CHK_BBOX_WIDTH_HOR;
            c.h = PLAYER_CHK_BBOX_HEIGHT_HOR;
        break;
        case PLAYER_FACE_UP:
            c.x = p->pos.x + PLAYER_CHK_BBOX_X_OFFSET_VER;
            c.y = p->pos.y + PLAYER_CHK_BBOX_Y_OFFSET_UP;
            c.w = PLAYER_CHK_BBOX_WIDTH_VER;
            c.h = PLAYER_CHK_BBOX_HEIGHT_VER;
        break;
        case PLAYER_FACE_LEFT:
            c.x = p->pos.x + PLAYER_CHK_BBOX_X_OFFSET_LEFT;
            c.y = p->pos.y + PLAYER_CHK_BBOX_Y_OFFSET_HOR;
            c.w = PLAYER_CHK_BBOX_WIDTH_HOR;
            c.h = PLAYER_CHK_BBOX_HEIGHT_HOR;
        break;
    }
    return c;
}
