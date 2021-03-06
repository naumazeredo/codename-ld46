#include <vector>

#include "externs.h"

namespace physics {

bool collide = false;

void debug_window() {
}

void setup() {
  //debug::add_window(debug_window);
}

void update() {
  f64 delta_time = game_time::get_frame_duration();

  for (auto& shop_place : shop_place_info.shop_places) {
    if (geom::point_inside_convex_polygon(player_info.position, shop_place.collider)) {
      player_info.position = geom::resolve_collision(player_info.position, shop_place.collider);
    }
  }
  for (auto& bound: bound_info.bounds) {
    if (geom::point_inside_convex_polygon(player_info.position, bound)) {
      player_info.position = geom::resolve_collision(player_info.position, bound);
    }
  }
}


} // namespace physics
