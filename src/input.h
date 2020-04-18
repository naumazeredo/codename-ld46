#pragma once

#include <SDL.h>

namespace input {

//void setup();
void update();
bool is_key_pressed(SDL_Scancode code);
bool is_key_down(SDL_Scancode code);
bool is_key_up(SDL_Scancode code);

}
