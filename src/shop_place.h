#pragma once

#include <vector>

#include "types.h"
#include "geom.h"

enum ShopPlaceState {OCCUPIED, FREE};

struct ShopPlace {
	geom::Rect trigger, collider;

	ShopPlaceState state;
  u32 shop_id;

	u32 texture;
};

struct ShopPlaceInfo {
	std::vector<ShopPlace> shop_places;
};

namespace shop_place {
 	void setup();
  void render();
}