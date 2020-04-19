#pragma once

#include "types.h"

struct PlayerInfo {

  Direction direction;
  u32 textures[NUM_DIRECTIONS];

  Point position;
  int w, h;
  int speed;
};

namespace player {

void setup();
void update();

}
