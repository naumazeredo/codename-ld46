#pragma once

#include <SDL.h>

#include "imgui.h"

namespace debug {

void setup(SDL_Window*, SDL_GLContext);
void cleanup();
void handle_input(SDL_Event*);

void render(SDL_Window*);

}
