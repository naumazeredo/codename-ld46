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
      ImGui::Text("Shop center: %d %d", shop_place.center.x, shop_place.center.y);
      ImGui::Text("Player inside: %s", geom::point_inside_convex_polygon(player_info.position, shop_place.collider) ? "YES" : "NO");
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
  geom::Point trigger_center = { 3.0f * SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2.0f - 100 };
  geom::Point trigger_h_offset = { 200, 0 };
  geom::Point trigger_v_offset = { 0, 200 };
  shop_place.trigger = { trigger_center - trigger_h_offset - trigger_v_offset,
                         trigger_center + trigger_h_offset - trigger_v_offset,
                         trigger_center + trigger_h_offset + trigger_v_offset,
                         trigger_center - trigger_h_offset + trigger_v_offset};
  geom::Point collider_center = { 3.0f * SCREEN_WIDTH / 4, SCREEN_HEIGHT / 2.0f - 100 };
  geom::Point collider_h_offset = { 100, 0 };
  geom::Point collider_v_offset = { 0, 75 };
  shop_place.collider = { collider_center - collider_h_offset - collider_v_offset,
                          collider_center + collider_h_offset - collider_v_offset,
                          collider_center + collider_h_offset + collider_v_offset,
                          collider_center - collider_h_offset + collider_v_offset};
  shop_place.w = 200; shop_place.h = 150;
  shop_place.center = collider_center;
  shop_place.state = ShopPlaceState::FREE;
  shop_place_info.shop_places.push_back(shop_place);
  printf("%zd\n", shop_place_info.shop_places.back().trigger.size());
}

void render() {
  for(auto& shop_place : shop_place_info.shop_places) {
    TextureCode texture = shop_place.texture;
    if(shop_place.state == ShopPlaceState::OCCUPIED) {
      auto [exist, shop_model] = shop::get_model_by_shop_id(shop_place.shop_id);

      if (exist)
        texture = shop_model.texture;
    }

    render::add_to_render(shop_place.center.x - shop_place.w / 2,
                          shop_place.center.y - shop_place.h / 2,
                          shop_place.w,
                          shop_place.h,
                          texture);
  }
}


} // namespace shop_place
