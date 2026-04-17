#include "textbox.h"
#include "types.h"
#include "bitflag.h"
#include "input.h"
#include "statusbf.h"
#include "utils.h"

static const String errorMsg = { "You FUCKED up the room messages\nIDIOT", 37 };

int InitTextbox(Textbox* tb)
{
    /* TEMP */
    tb->graphic = LoadImage("res-temp/textbox.png");
    if (!tb->graphic) return 1;
    tb->charsDrawn = 0;
    tb->shouldDraw = SDL_FALSE;
    return 0;
}

void UpdateTextbox(Textbox* tb, RoomMessage* msgs, Uint32 vPad, Uint32* status)
{
    if (tb->shouldDraw)
    {
        String msg;
        int next;
        if (tb->msgToDraw == -1)
        {
            msg = errorMsg;
            next = -1;
        }
        else
        {
            msg = msgs[tb->msgToDraw].msg;
            next = msgs[tb->msgToDraw].next;
        }
        /* NOTE assumes len is exact, doesn't do any null character checking */
        if (tb->charsDrawn >= msg.len)
        {
            if (CheckVInput(vPad, VKEY_ACCEPT) || CheckVInput(vPad, VKEY_MENU_HELD))
            {
                if (next >= 0) tb->msgToDraw = next;
                else
                {
                    tb->shouldDraw = SDL_FALSE;
                    ClearFlag(status, STATUS_IS_CUTSCENE);
                }
                tb->charsDrawn = 0;
            }
        }
        else
        {
            if (CheckVInput(vPad, VKEY_CANCEL) || CheckVInput(vPad, VKEY_MENU_HELD)) tb->charsDrawn = msg.len;
            else ++tb->charsDrawn; /* intentionally increment to 1 when first rendering, otherwise box will be blank */
        }
    }
}

int DrawTextbox(Textbox* tb, RoomMessage* msgs, SDL_bool isDW, Fonts* fnt, SDL_Surface* screen)
{
    int err = 0;
    if (tb->shouldDraw)
    {
        const Vec2 textStart = {29*2, 170*2};
        String msgSlice;
        int font, tbrecty;
        SDL_Rect tbrect;

        if (tb->msgToDraw == -1) msgSlice = errorMsg;
        else msgSlice = msgs[tb->msgToDraw].msg;
        font = (isDW)? FONT_MAIN_DW : FONT_MAIN_LW;
        tbrecty = (isDW)? 0 : 167;
        tbrect = NewSDL_Rect(0, tbrecty, 593, 167);
        err = BlitSurfaceCoords(tb->graphic, &tbrect, screen, NewVec2(24, 312));
        msgSlice.len = tb->charsDrawn;
        err = DrawText(msgSlice, fnt, screen, font, textStart);
    }
    return err;
}
