#include <vector>

#include "types.h"
#include "geometry.h"
#include "player.h"
#include "shop.h"
#include "time.h"
#include "externs.h"


namespace physics {

bool collide = false;

void debug_window() {}

void setup() {
    debug::add_window(debug_window);
}

void update() {
    f64 delta_time = time::get_frame_duration();

    for (auto& shop : shops_info.shops) {
        if (geometry::point_inside_Rect(player_info.position, shop.collider)) {
            Point out_right = { shop.center.x + shop.collider.width / 2 - player_info.position.x, 0 };
            Point out_left = { shop.center.x - shop.collider.width / 2 - player_info.position.x, 0 };
            Point out_up = { 0, shop.center.y + shop.collider.height / 2 - player_info.position.y };
            Point out_down = { 0, shop.center.y - shop.collider.height / 2 - player_info.position.y };
            
            Point escape = geometry::min_abs_point({out_right, out_left, out_up, out_down});

            player_info.position += escape;
        } 
    }
    for (auto& shop : shops_info.shops) {
        shop.state = geometry::point_inside_Rect(player_info.position, shop.trigger)
                        ? shop::OPEN 
                        : shop::CLOSED;
    }
}


} // namespace physics