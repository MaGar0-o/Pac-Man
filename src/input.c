#include "SDL2_gfxPrimitives.h"
#include "input.h"

Action convert_sdl_to_action(SDL_Keycode e) {
    switch (e) {
        case SDLK_UP:
            return ACTION_UP;
        case SDLK_DOWN:
            return ACTION_DOWN;
        case SDLK_LEFT:
            return ACTION_LEFT;
        case SDLK_RIGHT:
            return ACTION_RIGHT;
        default:
            return ACTION_NONE;
    }
}