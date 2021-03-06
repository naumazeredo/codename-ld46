#pragma once

#include <vector>
#include <map>

#include "texture.h"
#include "types.h"
#include "geom.h"

const u32 MONEY_PER_COIN = 5;
const u32 HEALTY_PER_CANDY = 50;

enum class ItemType {
  CANDY,
  MONEY,
  TRAP,
  TURRET,
  SHOP,
  FACTORY,
  UNPICKABLE
};

struct ItemModel {
  ItemType type;
  TextureCode texture, shadow;
  f32 texture_pivot_x, texture_pivot_y;
  u32 animation_set_id;
  u32 w, h;

  union {
    struct {
      u8 hunger_count;
    };
    struct {
      u8 damage;
      f64 action_rate;
      f64 action_range;
    };
    struct {
      u32 shop_model_id;
    };
  };
};

struct Item {
  u32 id;
  u32 model_id;
  bool being_held;
  geom::Point position;
  f64 last_action_time;
  u32 animation_instance_id;
};

struct ItemInfo {
  u32 num_item;
  std::vector<ItemModel> models;
  std::map<u32, Item> items;
};

namespace item {

void setup();
void cleanup();
void update();
void render();
void debug_window();

std::tuple<bool, ItemModel> get_model_by_item_id(u32 item_id);
std::tuple<bool, Item> get_item_by_id(u32 id);

u32 create_item(u32 model, geom::Point position);
void create_logs_line(u32 model_id, geom::Point base, int x_dir, int y_dir);
bool destroy_item(u32 id);

float dist_to_item(geom::Point position, u32 item_id);
u32 closest_item(geom::Point position); // @TODO(naum): return pattern std::tuple<bool, u32>

bool item_exists(u32 id);

bool update_position(u32 id, geom::Point position);

bool hold_item(u32 id);
bool drop_item(u32 id);

}
