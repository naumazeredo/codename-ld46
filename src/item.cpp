#include "item.h"

#include "render.h"

ItemInfo item_info;

namespace item {

void setup() {
  ItemModel tmp;

  tmp.type = FOOD;
  tmp.texture = render::load_image("assets/gfx/template-32x32-up.png");
  tmp.w = 30;
  tmp.h = 30;
  tmp.hunger_count = 5;
  item_info.models.push_back(tmp);

  tmp.type = WEAPON;
  tmp.texture = render::load_image("assets/gfx/template-32x32-right.png");
  tmp.w = 40;
  tmp.h = 30;
  tmp.damage = 10;
  item_info.models.push_back(tmp);
}

void update_position(u32 id, Point position) {
  if(item_info.items.find(id) == item_info.items.end()) return;

  Item &item = item_info.items[id];

  item.position = position;
}

u32 create_item(u32 model, Point position) {
  u32 id = ++item_info.num_item;
  u32 w = item_info.models[model].w;
  Item item;

  item.id = id;
  item.model = model;

  item.position = position;

  item_info.items[id] = item;

  return id;
}

void render() {
  for(auto p: item_info.items) {
    auto item = p.second;
    auto model = item_info.models[item.model];
    render::add_to_render(item.position.x - model.w/2, item.position.y, model.w, model.h, model.texture);
  }
}

}
