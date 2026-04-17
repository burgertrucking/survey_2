#ifndef ALL_C
#define ALL_C

#include "bitflag.c"
#include "input.c"
#include "player.c"
#include "room.c"
#include "textbox.c"
#include "textdraw.c"
#include "types.c"
#include "utils.c"

#define SDL_STBIMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_NO_LINEAR
#define STBI_NO_HDR
#include "SDL_stbi.h"

#include "pw_sdl.c"

#endif /* ALL_C */
