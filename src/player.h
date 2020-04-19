#pragma once

#include "types.h"

struct PlayerInfo {

  Direction direction;
  u32 textures[NUM_DIRECTIONS];

  f32 x, y;
  int w, h;
  int speed;
};

namespace player {

void setup();
void update();

}
