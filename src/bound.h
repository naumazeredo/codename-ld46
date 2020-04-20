#pragma once

#include <vector>
#include "geom.h"

struct BoundInfo {
  std::vector<geom::Polygon> bounds;
};

namespace bound {
  void setup();
}
