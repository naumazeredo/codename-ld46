#pragma once

#include <SDL.h>

#include "imgui.h"

void setup_debug(SDL_Window*, SDL_GLContext);
void cleanup_debug();
void handle_debug_input(SDL_Event*);

void render_debug_window(SDL_Window*);
