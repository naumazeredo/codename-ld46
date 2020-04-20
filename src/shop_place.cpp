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
      ImGui::Text("x %.2lf", shop_place.collider.x);
      ImGui::Text("y %.2lf", shop_place.collider.y);
      ImGui::Text("w %.2lf", shop_place.collider.w);
      ImGui::Text("h %.2lf", shop_place.collider.h);
      ImGui::Text("State: %s", shop_place.state == ShopPlaceState::FREE ? "FREE" : "OCCUPIED");
      if(shop_place.state == ShopPlaceState::OCCUPIED)
        ImGui::Text("Shop ID: %d", shop_place.shop_id);

    }

    ImGui::TreePop();
  }
}

void setup() {
  ShopPlace shop_place;
  shop_place.texture = TextureCode::TEX_BLANK;
  shop_place.trigger = { 3.0f * SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2.0f - 100, 200, 200};
  shop_place.collider = { 3.0f * SCREEN_WIDTH / 4 + 25, SCREEN_HEIGHT / 2.0f - 75, 150, 150};
  shop_place.state = ShopPlaceState::FREE;
  shop_place_info.shop_places.push_back(shop_place);
}

void render() {
  for(auto& shop_place : shop_place_info.shop_places) {
    TextureCode texture = shop_place.texture;
    if(shop_place.state == ShopPlaceState::OCCUPIED) {
      auto [exist, shop_model] = shop::get_model_by_shop_id(shop_place.shop_id);

      if (exist)
        texture = shop_model.texture;
    }

    render::add_to_render(shop_place.collider.x,
                          shop_place.collider.y,
                          shop_place.collider.w,
                          shop_place.collider.h,
                          texture);
  }
}


} // namespace shop_place
