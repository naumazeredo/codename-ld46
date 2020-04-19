#include "input.h"

#include <vector>
#include <algorithm>

#include "externs.h"

InputInfo input_info;

namespace input {

const Uint8 *state = SDL_GetKeyboardState(NULL);
Uint8 old_state[SDL_NUM_SCANCODES];

void setup() {
  input_info.direction_keys[0] = SDL_SCANCODE_UP;
  input_info.direction_keys[1] = SDL_SCANCODE_DOWN;
  input_info.direction_keys[2] = SDL_SCANCODE_LEFT;
  input_info.direction_keys[3] = SDL_SCANCODE_RIGHT;
}

void handle_input() {
  f64 delta_time = time::get_frame_duration();

  SDL_PumpEvents();

  for (int code = 0; code < SDL_NUM_SCANCODES; code++) {
    old_state[code] = state[code];
  }

  for(int i = 0; i < NUM_DIRECTIONS; i++) {
    if(input::is_key_pressed(input_info.direction_keys[i])) {
      player_info.position += Point(dx[player_info.direction] * player_info.speed * delta_time,
                                    dy[player_info.direction] * player_info.speed * delta_time);
      player_info.direction = (Direction) i;
    }
  }

}

bool is_key_pressed (SDL_Scancode code) { return state[code]; }
bool is_key_down    (SDL_Scancode code) { return (old_state[code] and !state[code]); }
bool is_key_up      (SDL_Scancode code) { return (!old_state[code] and state[code]); }

}
