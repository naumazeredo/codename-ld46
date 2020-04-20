#include "item.h"

#include "externs.h"

ItemInfo item_info;

namespace item {

void debug_window() {
  if (ImGui::TreeNode("Items")) {
    if (ImGui::TreeNode("Models")) {
      int model_id = 0;
      for (auto& model : item_info.models) {
        ImGui::PushID(&model);

        ImGui::Text("Model %d", model_id);
        model_id++;

        ImGui::SliderU32("texture", &model.texture, 0, render_info.texture.size()-1);
        ImGui::SliderU32("w", &model.w, 0, 128);
        ImGui::SliderU32("h", &model.h, 0, 128);

        switch (model.type) {
          case FOOD:
            ImGui::SliderU8("hunger count", &model.hunger_count, 0, 255);
          break;
          case TRAP:
            ImGui::SliderU8("damage", &model.damage, 0, 255);
          break;
        }

        ImGui::PopID();
      }

      ImGui::TreePop();
    }

    for (auto &[item_id, item] : item_info.items) {
      ImGui::Text("Item %d", item_id);
      ImGui::SliderU32("model", &item.model, 0, item_info.models.size()-1);
    }

    ImGui::TreePop();
  }
}

void setup() {
  debug::add_window(debug_window);
  ItemModel tmp;

  tmp.type = FOOD;
  tmp.texture = TEX_FOOD;
  tmp.w = 30;
  tmp.h = 30;
  tmp.hunger_count = 5;
  item_info.models.push_back(tmp);

  tmp.type = TRAP;
  tmp.texture = TEX_TRAP;
  tmp.w = 40;
  tmp.h = 30;
  tmp.damage = 10;
  item_info.models.push_back(tmp);

  tmp.type = TURRET;
  tmp.texture = TEX_TURRET;
  tmp.w = 40;
  tmp.h = 30;
  tmp.damage = 10;
  tmp.fire_rate = 1; // Shots Per Second
  item_info.models.push_back(tmp);

  tmp.type = SHOP;
  tmp.texture = TEX_SHOP;
  tmp.w = 40;
  tmp.h = 30;
  tmp.shop_model = 0;
  item_info.models.push_back(tmp);
}

bool exists_item(u32 id) {
  if (item_info.items.find(id) == item_info.items.end()) return false;
  return true;
}

bool destroy_item(u32 id) {
  if (!exists_item(id)) return false;
  item_info.items.erase(id);
  return true;
}

bool update_position(u32 id, geom::Point position) {
  if (!exists_item(id)) return false;

  Item &item = item_info.items[id];

  item.position = position;
  return true;
}

bool hold_item(u32 id) {
  if (!exists_item(id) || item_info.items[id].being_held) return false;
  item_info.items[id].being_held = true;

  return true;
}

bool drop_item(u32 id) {
  if (!exists_item(id) || !item_info.items[id].being_held) return false;
  item_info.items[id].being_held = false;

  return true;
}

void update() {
  for(auto &[item_id, item]: item_info.items) {
    if(item.being_held) continue;

    const auto &item_model = item_info.models[item.model];
    if (item_model.type == TRAP) {
      u32 enemy_id = enemy::closest_enemy_in(item.position, 15);
      if (enemy_id >= 0 && enemy::hit_enemy(enemy_id, item_model.damage)) {
        destroy_item(item_id);
      }
    }

    if (item_model.type == TURRET) {
      const f64 current_time = game_time::get_current_time();
      if(current_time < item.last_action_time + 1/item_model.fire_rate)
       continue;

      item.last_action_time = current_time;

      u32 enemy_id = enemy::closest_enemy_in(item.position, 70);
      enemy_id >= 0 && enemy::hit_enemy(enemy_id, item_model.damage);
    }
  }
}

float dist_to_item(geom::Point position, u32 item) {
  return (position - item_info.items[item].position).abs();
}

u32 closest_item(geom::Point position) {
  if(item_info.items.empty())
    return 0;
  u32 ans = item_info.items.begin()->first;
  float dist = dist_to_item(position, item_info.items.begin()->first);

  for (auto p: item_info.items) {
    float d = dist_to_item(position, p.first);

    if (d < dist) {
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
  item.last_action_time = game_time::get_current_time();
  item.being_held = false;

  item.position = position;

  item_info.items[id] = item;

  return id;
}

void render() {
  for (auto p: item_info.items) {
    auto item = p.second;
    auto model = item_info.models[item.model];
    render::add_to_render(item.position.x - model.w/2, item.position.y, model.w, model.h, model.texture);
  }
}

}
