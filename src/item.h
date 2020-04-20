#pragma once

#include <vector>
#include <map>

#include "geom.h"
#include "types.h"

enum ItemType {
  FOOD,
  TRAP,
  TURRET,
  SHOP
};

struct ItemModel {
  ItemType type;
  u32 texture;
  u32 w, h;

  union {
    struct {
      u8 hunger_count;
    };
    struct {
      u8 damage;
      f64 fire_rate;
    };
    struct {
      u32 shop_model;
    };
  };
};

struct Item {
  u32 id;
  u32 model;
  bool being_held;
  geom::Point position;
  f64 last_action_time;
};

struct ItemInfo {
  u32 num_item;
  std::vector<ItemModel> models;
  std::map<u32, Item> items;
};

namespace item {

void setup();

float dist_to_item(geom::Point position, u32 item);
u32 closest_item(geom::Point position);
u32 create_item(u32 model, geom::Point position);
bool destroy_item(u32 id);

bool exists_item(u32 id);
bool update_position(u32 id, geom::Point position);

void update();
void render();
bool hold_item(u32 id);
bool drop_item(u32 id);

}
