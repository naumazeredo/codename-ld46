#pragma once

#include "types.h"
#include "geom.h"

struct KingInfo {
  geom::Point position;
  u32 texture;

  int w, h;
};

namespace king {

void setup();
void render();
void debug_window();

}
