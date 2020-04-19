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

  for (auto& shop : shops_info.shops) {
    if (geom::point_inside_rect(player_info.position, shop.collider)) {
      geom::Point out_right = { shop.center.x + shop.collider.width / 2 - player_info.position.x, 0 };
      geom::Point out_left = { shop.center.x - shop.collider.width / 2 - player_info.position.x, 0 };
      geom::Point out_up = { 0, shop.center.y + shop.collider.height / 2 - player_info.position.y };
      geom::Point out_down = { 0, shop.center.y - shop.collider.height / 2 - player_info.position.y };

      geom::Point escape = geom::min_abs_point({out_right, out_left, out_up, out_down});

      player_info.position += escape;
    }
  }
  for (auto& shop : shops_info.shops) {
    shop.state = geom::point_inside_rect(player_info.position, shop.trigger)
      ? OPEN 
      : CLOSED;
  }
}


} // namespace physics
