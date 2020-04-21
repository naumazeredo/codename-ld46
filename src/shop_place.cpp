#include <vector>
#include <string>

#include <imgui.h>

#include "externs.h"

ShopPlaceInfo shop_place_info;

namespace shop_place {

void debug_window() {
  if (ImGui::TreeNode("Shop places")) {
    int cnt = 0;
    for(auto& shop_place : shop_place_info.shop_places) {
      std::string label = "Shops place: " + cnt++;

        ImGui::Point("Shop_place center", &shop_place.center);
      if (ImGui::TreeNode(label.c_str())) {
        ImGui::Text("center: %d %d", shop_place.center.x, shop_place.center.y);
        ImGui::Text("player inside? %s", geom::point_inside_convex_polygon(player_info.position, shop_place.collider) ? "YES" : "NO");

        ImGui::Text("state: %s", shop_place.state == ShopPlaceState::FREE ? "FREE" : "OCCUPIED");
        if(shop_place.state == ShopPlaceState::OCCUPIED)
          ImGui::Text("shop_id: %d", shop_place.shop_id);

        ImGui::TreePop();
      }
    }

    ImGui::TreePop();
  }
}

void setup() {
  create_shop_place({440, 153}, 200, 128);
  create_shop_place({640, 133}, 200, 128);
  create_shop_place({840, 153}, 200, 128);
  create_shop_place({440, 566}, 200, 128);
  create_shop_place({640, 586}, 200, 128);
  create_shop_place({840, 566}, 200, 128);
}

void cleanup() {
  shop_place_info.shop_places.clear();
}

void update() {
  f64 cur_time = game_time::get_time();

  for(auto& [id, shop] : shop_info.shops) {
      auto [have_, shop_model] = shop::get_model_by_shop_id(id);

      if(shop_model.type == ShopType::FACTORY){
        if(cur_time - shop.last_make_time >= shop_model.make_rate){
          shop.last_make_time = cur_time;
          auto shop_place = shop_place_info.shop_places[shop.shop_place_id];
          geom::Point drop_place = shop_place.center;

          if(drop_place.y > SCREEN_HEIGHT/2) drop_place.y -= 100;
          else drop_place.y += 50;

          drop_place.y += game::rand()%1000/(f64)100 - 5;
          drop_place.x += game::rand()%1000/(f64)100 - 5;

          item::create_item(shop_model.item_model_id, drop_place);
        }
    }
  }
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
                          texture,
                          shop_place.center.y - shop_place.h / 2);
  }
}

u32 create_shop_place(geom::Point center, u32 w, u32 h) {
  ShopPlace shop_place;

  shop_place.texture = TextureCode::TEX_CONSTRUCTION_SITE;
  shop_place.center = center;
  shop_place.w = w;
  shop_place.h = h;

  geom::Point trigger_center = center;
  geom::Point trigger_h_offset = { (f32)shop_place.w/2 + 10, 0 };
  geom::Point trigger_v_offset = { 0, (f32)shop_place.h/2 + 10 };
  shop_place.trigger = { trigger_center - trigger_h_offset - trigger_v_offset,
                         trigger_center + trigger_h_offset - trigger_v_offset,
                         trigger_center + trigger_h_offset + trigger_v_offset,
                         trigger_center - trigger_h_offset + trigger_v_offset};

  geom::Point collider_center = center;
  geom::Point collider_h_offset = { (f32)shop_place.w/2+2, 0 };
  geom::Point collider_v_offset = { 0,  (f32)shop_place.h/2+2};
  shop_place.collider = { collider_center - collider_h_offset - collider_v_offset,
                          collider_center + collider_h_offset - collider_v_offset,
                          collider_center + collider_h_offset + collider_v_offset,
                          collider_center - collider_h_offset + collider_v_offset};

  shop_place.state = ShopPlaceState::FREE;

  shop_place_info.shop_places.push_back(shop_place);

  return shop_place_info.shop_places.size()-1;
}

} // namespace shop_place
