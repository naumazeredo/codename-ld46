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
    if (geom::point_inside_rect(player_info.position, shop_place.collider)) {
      geom::Point out_right = { shop_place.collider.center.x + shop_place.collider.width / 2 - player_info.position.x, 0 };
      geom::Point out_left = { shop_place.collider.center.x - shop_place.collider.width / 2 - player_info.position.x, 0 };
      geom::Point out_up = { 0, shop_place.collider.center.y + shop_place.collider.height / 2 - player_info.position.y };
      geom::Point out_down = { 0, shop_place.collider.center.y - shop_place.collider.height / 2 - player_info.position.y };

      geom::Point escape = geom::min_abs_point({out_right, out_left, out_up, out_down});

      player_info.position += escape;
    }
  }
}


} // namespace physics
