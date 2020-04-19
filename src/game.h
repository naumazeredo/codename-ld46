#pragma once

#include <vector>
#include <functional>

#include "types.h"

const f64 KING_HUNGER_RATE = f64(1.0);
const f64 KING_MAX_HEALTH = f64(600.0);

enum GameState {
  RUNNING,
  GAME_OVER
};


using Callback = std::function<void(void)>;

struct GameInfo {
  f64 king_health;

  GameState current_state;

  std::vector<Callback> on_game_over;
  std::vector<Callback> on_damage_taken;
};

namespace game {
  void setup();
  void update();
  void render();
  void take_damage(f64);
}