#include <vector>
#include <string>

#include <imgui.h>

#include "externs.h"

ShopInfo shop_info;

namespace shop {

void debug_window() {
  if (ImGui::TreeNode("Shops")) {
    if (ImGui::TreeNode("Models")) {
      int model_id = 0;
      for (auto& model : shop_info.models) {
        ImGui::PushID(&model);

        ImGui::Text("Model %d", model_id++);


        //ImGui::SliderContainer("texture", &model.texture, render_info.texture);
        ImGui::Text("texture: %u", (u32)model.texture);
        ImGui::SliderU32("w", &model.w, 0, 128);
        ImGui::SliderU32("h", &model.h, 0, 128);

        ImGui::Text("item_model_id: %u", model.item_model_id);

        switch (model.type) {
          case ShopType::SHOP:
            ImGui::DragU32("sell_price", &model.sell_price);
          break;
          case ShopType::FACTORY:
            ImGui::DragFloat("make_rate", &model.make_rate);
          break;
        }

        ImGui::PopID();
      }

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Shop")) {
      for (auto &[id, shop] : shop_info.shops) {
        std::string label = "Shop: " + std::to_string(id);

        if (ImGui::TreeNode(label.c_str())) {
          ImGui::SliderContainer("model_id", &shop.model_id, shop_info.models);
          ImGui::Text("make_time: %lf", shop.last_make_time);

          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }

    ImGui::TreePop();
  }
}

void setup() {
  ShopModel tmp;

  tmp.type = ShopType::SHOP;
  tmp.texture = TextureCode::TEX_SHOP;
  tmp.item_model_id = 2;
  tmp.sell_price = 10;
  shop_info.models.push_back(tmp);

  tmp.type = ShopType::FACTORY;
  tmp.texture = TextureCode::TEX_FACTORY;
  tmp.item_model_id = 0;
  tmp.make_rate = 10;
  shop_info.models.push_back(tmp);
}

u32 create_shop(u32 model_id, u32 shop_place_id) {
  u32 id = ++shop_info.id_count;
  Shop shop;

  shop.id = id;
  shop.model_id = model_id;
  shop.shop_place_id = shop_place_id;
  shop.last_make_time = game_time::get_time();

  shop_info.shops[id] = shop;

  return id;
}

void destroy_shop(u32 shop_id) {
  if (!shop_info.shops.count(shop_id)) {
    printf("[error] Could not find shop (destroy_shop)!\n");
    return;
  }

  shop_info.shops.erase(shop_id);
}

std::tuple<bool, ShopModel> get_model_by_shop_id(u32 shop_id) {
  auto [shop_found, shop] = get_shop_by_id(shop_id);
  if(!shop_found) {
      return {false, {}};
  }

  u32 shop_model_id = shop.model_id;
  if(shop_model_id >= shop_info.models.size()) {
      return {false, {}};
  }
  return {true, shop_info.models[shop_model_id]};
}

std::tuple<bool, Shop> get_shop_by_id(u32 id) {
  if(!shop_info.shops.count(id)) {
    return {false, {}};
  }
  return {true, shop_info.shops[id]};
}

} // namespace shop
