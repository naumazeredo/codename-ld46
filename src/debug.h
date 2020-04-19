#pragma once

#include <vector>
#include <functional>

#include <SDL.h>

#include "imgui.h"

using DebugWindow = std::function<void(void)>;

struct DebugInfo {
  std::vector<DebugWindow> windows;
};

namespace debug {

void setup(SDL_Window*, SDL_GLContext);
void cleanup();

void handle_input(SDL_Event*);

void add_window(DebugWindow window);

void render(SDL_Window*);

}
