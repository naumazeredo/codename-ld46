#include "time.h"

#include <cstdio>

#include <SDL2/SDL.h>

TimeInfo time_info;

namespace time {

f64 get_current_time() {
  return SDL_GetPerformanceCounter() / f64(SDL_GetPerformanceFrequency());
}

void cap_framerate() {
  auto frame_duration = get_current_time() - time_info.real_time;

  if (frame_duration < FRAME_DURATION) {
    u32 delay_duration = 1000 * (FRAME_DURATION - frame_duration);
    SDL_Delay(delay_duration);
  }
}

void new_frame() {
  time_info.frame_count++;

  cap_framerate();

  time_info.real_frame_duration = get_current_time() - time_info.real_time;
  time_info.real_time += time_info.real_frame_duration;

  time_info.game_frame_duration = time_info.time_scale * time_info.real_frame_duration;
  time_info.game_time += time_info.game_frame_duration;
}

f64 get_frame_duration() {
  return time_info.game_frame_duration;
}

}
