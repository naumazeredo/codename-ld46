#pragma once

#include "types.h"

const u8 FRAMES_PER_SECOND = 60;
const f64 FRAME_DURATION = f64(1.0) / FRAMES_PER_SECOND;

// @TODO(naum): game time, game state
struct TimeInfo {
  u32 frame_count;

  f64 real_time;
  f64 real_frame_duration;

  f64 game_time;
  f64 game_frame_duration;

  f64 time_scale = 1.0;
};

namespace game_time {

void debug_window();

void new_frame();

f64 get_frame_duration();
f64 get_real_time();
f64 get_time();

}
