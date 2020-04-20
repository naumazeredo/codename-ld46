#include <imgui.h>
#include <vector>

#include "shop.h"
#include "render.h"
#include "debug.h"
#include "texture.h"

ShopInfo shop_info;

namespace shop {

void debug_window() {
  
}

void setup() {
  // debug::add_window(debug_window);
  ShopModel tmp;

  tmp.type = ShopType::SHOP;
  tmp.texture = TextureCode::TEX_SHOP;
  tmp.item_model_id = 2;
  tmp.sell_price = 10;

  shop_info.shop_models.push_back(tmp);
}

u32 create_shop(u32 model_id) {
  u32 id = ++shop_info.id_count;
  Shop shop;

  shop.id = id;
  shop.model_id = model_id;

  shop_info.shops[id] = shop;

  return id;
}

std::tuple<bool, ShopModel> get_model_by_shop_id(u32 shop_id) {
  auto [shop_found, shop] = get_shop_by_id(shop_id);
  if(!shop_found) {
      return {false, {}};
  }

  u32 shop_model_id = shop.model_id;
  if(shop_model_id >= shop_info.shop_models.size()) {
      return {false, {}};
  }
  return {true, shop_info.shop_models[shop_model_id]};
}

std::tuple<bool, Shop> get_shop_by_id(u32 id) {
  if(!shop_info.shops.count(id)) {
    return {false, {}};
  }
  return {true, shop_info.shops[id]};
}

} // namespace shop
