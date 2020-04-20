#include "input.h"

#include <iostream>
#include <vector>
#include <algorithm>

#include "externs.h"

InputInfo input_info;

namespace input {

Uint8 cur_state[SDL_NUM_SCANCODES];
Uint8 old_state[SDL_NUM_SCANCODES];

void debug_window() {
  if (ImGui::TreeNode("Input")) {
    ImGui::Text("U %d", cur_state[input_info.direction_keys[0]]);
    ImGui::Text("D %d", cur_state[input_info.direction_keys[1]]);
    ImGui::Text("L %d", cur_state[input_info.direction_keys[2]]);
    ImGui::Text("R %d", cur_state[input_info.direction_keys[3]]);

    ImGui::Text("action %d", cur_state[input_info.action_key]);

    ImGui::TreePop();
  }
}

void setup() {
  input_info.direction_keys[0] = SDL_SCANCODE_UP;
  input_info.direction_keys[1] = SDL_SCANCODE_DOWN;
  input_info.direction_keys[2] = SDL_SCANCODE_LEFT;
  input_info.direction_keys[3] = SDL_SCANCODE_RIGHT;

  input_info.action_key = SDL_SCANCODE_SPACE;
}

void handle_input() {
  f64 delta_time = game_time::get_frame_duration();

  for (int code = 0; code < SDL_NUM_SCANCODES; code++)
    old_state[code] = cur_state[code];

  SDL_PumpEvents();

  const Uint8 *new_state = SDL_GetKeyboardState(NULL);
  for (int code = 0; code < SDL_NUM_SCANCODES; code++)
    cur_state[code] = new_state[code];

  for(int i = 0; i < NUM_DIRECTIONS; i++) {
    if(input::is_key_pressed(input_info.direction_keys[i])) {
      player_info.position += geom::Point (dx[(int)player_info.direction] * player_info.speed * delta_time,
                                           dy[(int)player_info.direction] * player_info.speed * delta_time);
      player_info.direction = (Direction) i;
    }
  }

  if(is_key_down(input_info.action_key)) {
    player::item_interaction();
  }
}

bool is_key_pressed (SDL_Scancode code) { return cur_state[code]; }
bool is_key_down    (SDL_Scancode code) { return (old_state[code] and !cur_state[code]); }
bool is_key_up      (SDL_Scancode code) { return (!old_state[code] and cur_state[code]); }

}
