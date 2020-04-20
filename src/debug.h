#pragma once

#include <vector>
#include <functional>

#include <SDL.h>
#include <imgui.h>

#include "geom.h"
#include "types.h"

using DebugWindow = std::function<void(void)>;

struct DebugInfo {
  bool show_window;
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

inline void DragU8 (const char* label, u8*  v) { DragScalar(label, ImGuiDataType_U8,  v, 1); }
inline void DragU16(const char* label, u16* v) { DragScalar(label, ImGuiDataType_U16, v, 1); }
inline void DragU32(const char* label, u32* v) { DragScalar(label, ImGuiDataType_U32, v, 1); }

inline void InputU8 (const char* label, u8*  v) { InputScalar(label, ImGuiDataType_U8,  v); }
inline void InputU16(const char* label, u16* v) { InputScalar(label, ImGuiDataType_U16, v); }
inline void InputU32(const char* label, u32* v) { InputScalar(label, ImGuiDataType_U32, v); }

inline void SliderU8 (const char* label, u8*  v, u8  v_min, u8  v_max, const char* format = "%u") { SliderScalar(label, ImGuiDataType_U8,  v, &v_min, &v_max, format); }
inline void SliderU16(const char* label, u32* v, u32 v_min, u32 v_max, const char* format = "%u") { SliderScalar(label, ImGuiDataType_U16, v, &v_min, &v_max, format); }
inline void SliderU32(const char* label, u32* v, u32 v_min, u32 v_max, const char* format = "%u") { SliderScalar(label, ImGuiDataType_U32, v, &v_min, &v_max, format); }

template<typename C>
inline void SliderContainer(const char* label, u32* v, C& c) {
  ImGui::SliderU32(label, v, 0, c.size()-1);
}

inline void Point(const char* label, geom::Point* p) {
  Text(label);
  DragFloat("x", &p->x);
  DragFloat("y", &p->y);
}

inline void Rect(const char* label, geom::Rect* r) {
  Text(label);
  DragFloat("x", &r->x);
  DragFloat("y", &r->y);
  DragFloat("w", &r->w);
  DragFloat("h", &r->h);
}

}
