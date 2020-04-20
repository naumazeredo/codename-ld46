#include "time.h"

#include <cstdio>

#include <SDL2/SDL.h>
#include <imgui.h>

TimeInfo time_info;

namespace game_time {

void debug_window() {
  ImGui::InputDouble("time_scale", &time_info.time_scale, 0.0, 8.0);

  if (ImGui::Button("pause")) {
    time_info.time_scale = 0;
  }

  ImGui::SameLine();

  if (ImGui::Button("play")) {
    time_info.time_scale = 1;
  }
}

f64 get_real_time() {
  return SDL_GetPerformanceCounter() / f64(SDL_GetPerformanceFrequency());
}

f64 get_time() {
  return time_info.game_time;
}

void cap_framerate() {
  auto frame_duration = get_real_time() - time_info.real_time;

  if (frame_duration < FRAME_DURATION) {
    u32 delay_duration = 1000 * (FRAME_DURATION - frame_duration);
    SDL_Delay(delay_duration);
  }
}

u32 get_current_frame() {
  return time_info.frame_count;
}

void new_frame() {
  time_info.frame_count++;

  cap_framerate();

  time_info.real_frame_duration = get_real_time() - time_info.real_time;
  time_info.real_time += time_info.real_frame_duration;

  time_info.game_frame_duration = time_info.time_scale * time_info.real_frame_duration;
  time_info.game_time += time_info.game_frame_duration;
}

f64 get_frame_duration() {
  return time_info.game_frame_duration;
}

}
