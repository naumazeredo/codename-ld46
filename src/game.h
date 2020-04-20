#pragma once

#include <vector>
#include <functional>
#include <random>

#include "texture.h"
#include "types.h"
#include "geom.h"

const f64 KING_HUNGER_RATE = f64(1.0);
const f64 KING_MAX_HEALTH = f64(600.0);

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
  f64 wave_remaining_time, wave_time;
  u32 last_wave_cnt, wave_enemy_increase;
  std::mt19937_64 rand;

  std::vector<Callback> on_game_over;
};

namespace game {

void setup();
void update();
void render();
void debug_window();

void take_damage(f64);
void spawn_wave(u32 enemy_count);
void feed_king(f64);

}
