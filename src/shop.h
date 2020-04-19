#pragma once

#include <vector>

#include "types.h"
#include "geom.h"

enum ShopState {CLOSED, OPEN};

struct Shop {
  geom::Point center;
  geom::Rect collider, trigger;

  ShopState state;

  u32 textures[2];

  Shop(geom::Point p, geom::Rect c, geom::Rect t);

  void sell();
};

struct ShopsInfo {
    std::vector<Shop> shops;
};

namespace shop {

void setup();
void update();

} // namespace shop

