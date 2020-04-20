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
  tmp.texture = TEX_SHOP;
  tmp.item = 2;
  tmp.sell_price = 10;

  shop_info.shop_models.push_back(tmp);
}

u32 create_shop(u32 model) {
  u32 id = ++shop_info.id_count;
  Shop shop;

  shop.id = id;
  shop.model = model;

  shop_info.shops[id] = shop;

  return id;
}

} // namespace shop
