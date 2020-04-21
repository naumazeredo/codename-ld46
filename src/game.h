#pragma once

#include <vector>
#include <functional>
#include <random>

#include "texture.h"
#include "types.h"
#include "geom.h"

enum class GameState {
  RUNNING,
  GAME_OVER
};

using Callback = std::function<void(void)>;

struct GameInfo {
  GameState current_state;

  TextureCode bar_texture;
  geom::Point bar_position;
  int bar_h, bar_w;

  std::vector<geom::Rect> spawn_pool;
  f64 wave_remaining_time, wave_time, time_until_next_wave;
  u32 last_wave_cnt, wave_enemy_increase;
  std::mt19937_64 rand;

  std::vector<Callback> on_game_over;
};

namespace game {

void setup();
void update();
void render();
void debug_window();

void spawn_wave(u32 enemy_count);

u64 rand();

}
