#pragma once

#include <vector>
#include <map>

#include "types.h"
#include "geom.h"

enum class ShopType {SHOP, FACTORY};

struct ShopModel {
	ShopType type;
  u32 texture;
  u32 w, h;
  u32 item_model_id;

  union {
    struct {
      u32 sell_price;
    };
    struct {
      u32 make_rate;
    };
  };
};

struct Shop {
  u32 id;
  u32 model;
};

struct ShopInfo {
  u32 id_count;
  std::vector<ShopModel> shop_models;
  std::map<u32, Shop> shops;
};

namespace shop {

void setup();
void update();

u32 create_shop(u32 model);

std::tuple<bool, ShopModel> get_model_by_shop_id(u32 shop_id);
std::tuple<bool, Shop> get_shop_by_id(u32 id);

} // namespace shop

