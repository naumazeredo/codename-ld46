#include <imgui.h>
#include <vector>

#include "externs.h"

ShopPlaceInfo shop_place_info;

namespace shop_place {

void debug_window() {
  if (ImGui::TreeNode("Shop Places")) {
    int cnt = 0;
    for(auto& shop_place : shop_place_info.shop_places) {
      ImGui::Text("Shop place %d: ", cnt++);
      ImGui::Text("x %.2lf", shop_place.collider.center.x);
      ImGui::Text("y %.2lf", shop_place.collider.center.y);
      ImGui::Text("w %.2lf", shop_place.collider.width);
      ImGui::Text("h %.2lf", shop_place.collider.height);
      ImGui::Text("State: %s", shop_place.state ? "FREE" : "OCCUPIED");
      if(shop_place.state == OCCUPIED)
        ImGui::Text("Shop ID: %d", shop_place.shop_id);

    }
    //ImGui:: enum
    ImGui::TreePop();
  }
}

void setup() {
  debug::add_window(debug_window);
  ShopPlace shop_place;
  shop_place.texture = render::load_image("assets/gfx/template-32x32.png");
  shop_place.trigger = {{ 3.0f * SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2.0f }, 400, 400};
  shop_place.collider = {{ 3.0f * SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2.0f }, 300, 300};
  shop_place.state = FREE;
  shop_place_info.shop_places.push_back(shop_place);
}

void render() {
  for(auto& shop_place : shop_place_info.shop_places) {
    u32 texture = shop_place.texture;
    if(shop_place.state == OCCUPIED)
      texture = shop_info.shop_models[shop_info.shops[shop_place.shop_id].model].texture;
    
    render::add_to_render(shop_place.collider.center.x - shop_place.collider.width / 2, 
                          shop_place.collider.center.y - shop_place.collider.height / 2, 
                          shop_place.collider.width, 
                          shop_place.collider.height, 
                          texture);
  }
}


} // namespace shop_place