#pragma once

#include <functional>

#include "texture.h"
#include "types.h"
#include "geom.h"

using Callback = std::function<void(void)>;

struct KingInfo {
  f64 king_health;
  geom::Point position;
  TextureCode texture;

  int w, h;

  u32 animation_set_id;

  std::vector<Callback> on_damage_taken;
  std::vector<Callback> on_feed_king;
};

namespace king {

u32 get_king_health();
geom::Point get_position();
void take_damage(f64 damage);

void setup();
void update();
void debug_window();

}
