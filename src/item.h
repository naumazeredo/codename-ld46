#include <vector>
#include <map>

#include "types.h"

enum ItemType {FOOD, WEAPON, TRAP};

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
    };
  };
};

struct Item {
  u32 id;
  u32 model;
  Point position;
};

struct ItemInfo {
  u32 num_item;
  std::vector<ItemModel> models;
  std::map<u32, Item> items;
};

namespace item {

void setup();
u32 create_item(u32 model, Point position);
void update_position(u32 id, Point position);
void render();

}
