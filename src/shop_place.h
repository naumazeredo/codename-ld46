#pragma once

#include <vector>

#include "texture.h"
#include "types.h"
#include "geom.h"

enum class ShopPlaceState {
  OCCUPIED,
  FREE
};

struct ShopPlace {
  TextureCode texture;

  geom::Point center;
  geom::Polygon trigger, collider;

  ShopPlaceState state;
  u32 shop_id;
  u32 w, h;
};

struct ShopPlaceInfo {
  std::vector<ShopPlace> shop_places;
};

namespace shop_place {

void setup();
void render();
void debug_window();
void update();

}
