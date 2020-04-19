#pragma once

#include <vector>
#include <functional>

#include <SDL.h>
#include <imgui.h>

#include "types.h"

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

namespace ImGui {

inline void SliderU32(const char* label, u32* v, u32 v_min, u32 v_max, const char* format = "%u") {
  SliderScalar(label, ImGuiDataType_U32, v, &v_min, &v_max, format);
}

inline void SliderU8(const char* label, u8* v, u8 v_min, u8 v_max, const char* format = "%u") {
  SliderScalar(label, ImGuiDataType_U8, v, &v_min, &v_max, format);
}

}
