#include "input.h"
#include "bitflag.h"

KeyBinds GetDefaultKeyBinds(void)
{
    return (KeyBinds){
        .up = SDLK_UP, .down = SDLK_DOWN, .left = SDLK_LEFT, .right = SDLK_RIGHT, .accept = SDLK_z, .cancel = SDLK_x, .menu = SDLK_c,
        .acceptA = SDLK_RETURN, .cancelA = SDLK_RSHIFT, .menuA = SDLK_RCTRL,
        .acceptB = SDLK_KP_ENTER, .cancelB = SDLK_LSHIFT, .menuB = SDLK_LCTRL,
    };
}

void PressVInput(Uint32* vPad, InputButton b)
{
    SetFlag(vPad, (Uint32)b);
}

void UnpressVInput(Uint32* vPad, InputButton b)
{
    ClearFlag(vPad, (Uint32)b);
}

SDL_bool CheckVInput(Uint32 vPad, InputButton b)
{
    InputButton ba = 0, bb = 0;
    switch (b)
    {
        case VKEY_UP:
        case VKEY_DOWN:
        case VKEY_LEFT:
        case VKEY_RIGHT:
        case VKEY_UP_HELD:
        case VKEY_DOWN_HELD:
        case VKEY_LEFT_HELD:
        case VKEY_RIGHT_HELD:
            /* no aliases */
        break;

        case VKEY_ACCEPT:
            ba = VKEY_ACCEPT_A;
            bb = VKEY_ACCEPT_B;
        break;

        case VKEY_CANCEL:
            ba = VKEY_CANCEL_A;
            bb = VKEY_CANCEL_B;
        break;

        case VKEY_MENU:
            ba = VKEY_MENU_A;
            bb = VKEY_MENU_B;
        break;

        case VKEY_ACCEPT_HELD:
            ba = VKEY_ACCEPT_A_HELD;
            bb = VKEY_ACCEPT_B_HELD;
        break;

        case VKEY_CANCEL_HELD:
            ba = VKEY_CANCEL_A_HELD;
            bb = VKEY_CANCEL_B_HELD;
        break;

        case VKEY_MENU_HELD:
            ba = VKEY_MENU_A_HELD;
            bb = VKEY_MENU_B_HELD;
        break;

        default:
            printf("WARNING: CheckVInput: Passed unsupported input type %i\n", b);
        break;
    }

    return CheckFlag(vPad, b) || CheckFlag(vPad, ba) || CheckFlag(vPad, bb);
}
