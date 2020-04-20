#include "bound.h"

#include <vector>
#include "externs.h"

BoundInfo bound_info;

namespace bound {

void setup() {
  geom::Polygon tmp;

  tmp.push_back({0, 0});
  tmp.push_back({0, SCREEN_HEIGHT});
  tmp.push_back({-30, SCREEN_HEIGHT});
  tmp.push_back({-30, 0});
  bound_info.bounds.push_back(tmp);

  tmp.clear();
  tmp.push_back({SCREEN_WIDTH + 30, 0});
  tmp.push_back({SCREEN_WIDTH + 30, SCREEN_HEIGHT});
  tmp.push_back({SCREEN_WIDTH, SCREEN_HEIGHT});
  tmp.push_back({SCREEN_WIDTH, 0});
  bound_info.bounds.push_back(tmp);
}

}
