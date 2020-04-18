#pragma once

#include<SDL.h>

void setup_input();
void update_keyboard();
bool is_pressed(SDL_Scancode code);
bool unpress(SDL_Scancode code);
bool press(SDL_Scancode code);
