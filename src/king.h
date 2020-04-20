#pragma once

#include <functional>

#include "texture.h"
#include "types.h"
#include "geom.h"

const u8 STORE_DROP_RATE = 20; // Percentage
const f64 KING_HUNGER_RATE = f64(3.0);
const f64 KING_MAX_HEALTH = f64(600.0);

using Callback = std::function<void(void)>;

struct KingInfo {
  f64 health;
  geom::Point position;
  geom::Point texture_pivot;
  std::vector<u64> drop_model_ids;

  int w, h;

  u32 animation_instance_id;

  std::vector<Callback> on_damage_taken;
  std::vector<Callback> on_feed_king;
};

namespace king {

u32 get_health();
geom::Point get_position();
void take_damage(f64 damage);
void feed_king(f64 amount);

void setup();
void update();
void render();
void debug_window();

}
