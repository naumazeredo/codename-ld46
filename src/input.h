#pragma once

#include <SDL.h>

#include "types.h"

struct InputInfo {
   SDL_Scancode direction_keys[NUM_DIRECTIONS];
   SDL_Scancode action_key;
};

namespace input {

void setup();
void handle_input();
bool is_key_pressed(SDL_Scancode code);
bool is_key_down(SDL_Scancode code);
bool is_key_up(SDL_Scancode code);

void debug_window();

}
