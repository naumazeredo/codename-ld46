#include "input.h"

#include<vector>
#include<algorithm>
#include<iostream>

const Uint8 *state = SDL_GetKeyboardState(NULL);
Uint8 old_state[SDL_NUM_SCANCODES];

void update_keyboard() {
  SDL_PumpEvents();

  for (int code = 0; code < SDL_NUM_SCANCODES; code++) {
    old_state[code] = state[code];
  }
}

bool is_pressed (SDL_Scancode code) { return state[code]; }
bool unpress    (SDL_Scancode code) { return (old_state[code] and !state[code]); }
bool press      (SDL_Scancode code) { return (!old_state[code] and state[code]); }

