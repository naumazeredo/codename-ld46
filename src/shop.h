#pragma once

#include <vector>
#include <map>

#include "texture.h"
#include "types.h"
#include "geom.h"

enum class ShopType {SHOP, FACTORY};

struct ShopModel {
  TextureCode texture;
  u32 w, h;
  u32 item_model_id;

  ShopType type;
  union {
    struct {
      u32 sell_price;
    };
    struct {
      f32 make_rate;
    };
  };
};

struct Shop {
  u32 id;
  u32 model_id;
  u32 shop_place_id;
  f64 last_make_time;
};

struct ShopInfo {
  u32 id_count;
  std::vector<ShopModel> models;
  std::map<u32, Shop> shops;
};

namespace shop {

void setup();
void cleanup();
void update();
void debug_window();

u32 create_shop(u32 model, u32 shop_place_id);
void destroy_shop(u32 shop_id);

std::tuple<bool, ShopModel> get_model_by_shop_id(u32 shop_id);
std::tuple<bool, Shop> get_shop_by_id(u32 id);

} // namespace shop

