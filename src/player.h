#pragma once

#include "types.h"
#include "geom.h"

struct PlayerInfo {

  Direction direction;
  u32 textures[NUM_DIRECTIONS];
  u32 item;

  geom::Point position;
  geom::Point item_position;

  int w, h;
  int speed;
  float item_max_dist;
};

namespace player {

void setup();
void render();
void update();
void item_interaction();

}
