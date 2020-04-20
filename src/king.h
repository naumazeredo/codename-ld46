#pragma once

#include <functional>
#include "types.h"
#include "geom.h"

using Callback = std::function<void(void)>;

const f64 KING_MAX_HEALTH = f64(600.0);

struct KingInfo {
  f64 king_health;
  geom::Point position;
  u32 texture;
  int w, h;

  std::vector<Callback> on_damage_taken;
  std::vector<Callback> on_feed_king;
};

namespace king {

u32 get_king_health();
geom::Point get_position();
void take_damage(f64 damage);

void setup();
void render();
void debug_window();

}
