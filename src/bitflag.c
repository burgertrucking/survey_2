#include "bitflag.h"

void SetFlag(Uint32* flags, Uint32 val)
{
    *flags |= val;
}

void ClearFlag(Uint32* flags, Uint32 val)
{
    *flags &= ~val;
}

void ToggleFlag(Uint32* flags, Uint32 val)
{
    *flags ^= val;
}

SDL_bool CheckFlag(Uint32 flags, Uint32 val)
{
    return (flags & val) != 0;
}
