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

bool exists_item(u32 id) {
  if(item_info.items.find(id) == item_info.items.end()) return false;
  return true;
}

bool update_position(u32 id, Point position) {
  if(item_info.items.find(id) == item_info.items.end()) return false;

  Item &item = item_info.items[id];

  item.position = position;
  return true;
}

float dist_to_item(Point position, u32 item) {
  return (position - item_info.items[item].position).abs();
}

u32 closest_item(Point position) {
  u32 ans = item_info.items.begin()->first;
  float dist = dist_to_item(position, item_info.items.begin()->first);

  for(auto p: item_info.items) {
    float d = dist_to_item(position, p.first);

    if(d < dist) {
      dist = d;
      ans = p.first;
    }
  }

  return ans;
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
