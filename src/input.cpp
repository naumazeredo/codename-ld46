#include "input.h"

#include<vector>
#include<algorithm>
#include<iostream>

const Uint8 *state = SDL_GetKeyboardState(NULL);
Uint8 *old_state;

SDL_Scancode used_keys[] = {SDL_SCANCODE_RIGHT,
                            SDL_SCANCODE_LEFT,
                            SDL_SCANCODE_DOWN,
                            SDL_SCANCODE_UP};
int used_keys_size;

void setup_input() {
  used_keys_size = sizeof(used_keys)/sizeof(SDL_Scancode);

  int ma = used_keys[0];
  for(int i = 0; i < used_keys_size; i++) {
    ma = std::max(ma, (int)used_keys[i]);
  }
  old_state = (Uint8 *) malloc(ma+1*sizeof(Uint8));
}
void update_keyboard() {
  for(int i = 0; i < used_keys_size; i++) {
    old_state[used_keys[i]] = state[used_keys[i]];
  }
  SDL_PumpEvents();
}

bool is_pressed(SDL_Scancode code) {
  return state[code];
}
bool unpress(SDL_Scancode code) {
  return (old_state[code] and !state[code]);
}
bool press(SDL_Scancode code) {
  return (!old_state[code] and state[code]);
}

