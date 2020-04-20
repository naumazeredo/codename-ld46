#pragma once

#include <vector>
#include <functional>

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

  std::vector<Callback> on_game_over;
};

namespace game {

void setup();
void update();
void render();
void debug_window();

void take_damage(f64);
void feed_king(f64);

}
