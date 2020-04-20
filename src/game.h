#pragma once

#include <vector>
#include <functional>

#include "types.h"
#include "geom.h"

enum GameState {
  RUNNING,
  GAME_OVER
};

using Callback = std::function<void(void)>;

struct GameInfo {
  GameState current_state;

  u32 bar_texture;
  geom::Point bar_position;
  int bar_h, bar_w;

  std::vector<Callback> on_game_over;
};

namespace game {
  void setup();
  void update();
  void render();
  void take_damage(f64);
  void feed_king(f64);
}
