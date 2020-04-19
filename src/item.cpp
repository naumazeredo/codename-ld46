#include "item.h"

#include <string>
#include "render.h"
#include "enemy.h"
#include "debug.h"

ItemInfo item_info;

namespace item {

void debug_window() {
  if (ImGui::CollapsingHeader("Items")) {
    ImGui::Indent(10);
    for(const auto &[item_id, item] : item_info.items) {
      const std::string text = std::to_string(item_id) + " - Model: " + std::to_string(item.model);
      ImGui::Text(text.c_str());
    }
    ImGui::Indent(-10);
  }
}

void setup() {
  debug::add_window(debug_window);
  ItemModel tmp;

  tmp.type = FOOD;
  tmp.texture = render::load_image("assets/gfx/template-32x32-up.png");
  tmp.w = 30;
  tmp.h = 30;
  tmp.hunger_count = 5;
  item_info.models.push_back(tmp);

  tmp.type = TRAP;
  tmp.texture = render::load_image("assets/gfx/trap-must-replace.png");
  tmp.w = 40;
  tmp.h = 30;
  tmp.damage = 10;
  item_info.models.push_back(tmp);
}

bool exists_item(u32 id) {
  if(item_info.items.find(id) == item_info.items.end()) return false;
  return true;
}

bool destroy_item(u32 id) {
  if(!exists_item(id)) return false;
  item_info.items.erase(id);
  return true;
}

bool update_position(u32 id, geom::Point position) {
  if(!exists_item(id)) return false;

  Item &item = item_info.items[id];

  item.position = position;
  return true;
}

void update() {
  for(auto &[item_id, item]: item_info.items) {
    const auto &item_model = item_info.models[item.model];
    if (item_model.type == TRAP) {
      u32 enemy_id = enemy::closest_enemy_in(item.position, 30);
      if(enemy_id >= 0 && enemy::hit_enemy(enemy_id, item_model.damage)) {
        destroy_item(item_id);
      }
    }
  }
}

float dist_to_item(geom::Point position, u32 item) {
  return (position - item_info.items[item].position).abs();
}

u32 closest_item(geom::Point position) {
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

u32 create_item(u32 model, geom::Point position) {
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
