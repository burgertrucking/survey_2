#include "input.h"
#include "bitflag.h"

KeyBinds GetDefaultKeyBinds(void)
{
    KeyBinds result;
    result.up = SDLK_UP; result.down = SDLK_DOWN; result.left = SDLK_LEFT; result.right = SDLK_RIGHT;
    result.accept = SDLK_z; result.cancel = SDLK_x; result.menu = SDLK_c;
    result.acceptA = SDLK_RETURN; result.cancelA = SDLK_RSHIFT; result.menuA = SDLK_RCTRL;
    result.acceptB = SDLK_KP_ENTER; result.cancelB = SDLK_LSHIFT; result.menuB = SDLK_LCTRL;
    return result;
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
