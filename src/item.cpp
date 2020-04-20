#include "item.h"

#include <string>

#include "externs.h"

ItemInfo item_info;

namespace item {

void debug_window() {
  if (ImGui::TreeNode("Items")) {
    if (ImGui::TreeNode("Models")) {
      int model_id = 0;
      for (auto& model : item_info.models) {
        ImGui::PushID(&model);

        ImGui::Text("Model %d", model_id++);

        ImGui::SliderContainer("texture", &model.texture, render_info.texture);
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

    if (ImGui::TreeNode("Items")) {
      for (auto &[item_id, item] : item_info.items) {
        std::string label = "Item: " + std::to_string(item_id);

        if (ImGui::TreeNode(label.c_str())) {
          ImGui::SliderContainer("model", &item.model, item_info.models);
          ImGui::Point("position", &item.position);
          ImGui::Checkbox("being_held", &item.being_held);
          //f64 last_action_time;

          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }

    ImGui::TreePop();
  }
}

void setup() {
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
  tmp.damage = 1;
  tmp.fire_rate = 1; // Shots Per Second
  item_info.models.push_back(tmp);

  tmp.type = SHOP;
  tmp.texture = TEX_SHOP;
  tmp.w = 40;
  tmp.h = 30;
  tmp.shop_model = 0;
  item_info.models.push_back(tmp);
}

std::tuple<bool, ItemModel> get_model_by_item_id(u32 item_id) {
  auto [item_found, item] = get_item_by_id(item_id);
  if (!item_found) {
    return {false, {}};
  }
  u32 item_model_id = item.model;
  if(item_model_id >= item_info.models.size()) {
    return {false, {}};
  }
  return {true, item_info.models[item_model_id]};
}

std::tuple<bool, Item> get_item_by_id(u32 id) {
  if(!item_info.items.count(id)) {
    return {false, {}};
  }
  return {true, item_info.items[id]};
}

bool item_exists(u32 id) {
  if (item_info.items.find(id) == item_info.items.end()) return false;
  return true;
}

bool destroy_item(u32 id) {
  if (!item_exists(id)) return false;
  item_info.items.erase(id);
  return true;
}

bool update_position(u32 id, geom::Point position) {
  if (!item_exists(id)) return false;

  Item &item = item_info.items[id];

  item.position = position;
  return true;
}

bool hold_item(u32 id) {
  if (!item_exists(id) || item_info.items[id].being_held) return false;
  item_info.items[id].being_held = true;

  return true;
}

bool drop_item(u32 id) {
  if (!item_exists(id) || !item_info.items[id].being_held) return false;
  item_info.items[id].being_held = false;

  return true;
}

void update() {
  for(auto &[item_id, item]: item_info.items) {
    if(item.being_held) continue;

    const auto &item_model = item_info.models[item.model];
    if (item_model.type == TRAP) {
      auto [has_enemy, enemy_id] = enemy::closest_enemy_in(item.position, 15);
      if (has_enemy && enemy::try_hit_enemy(enemy_id, item_model.damage)) {
        destroy_item(item_id);
      }
    }

    if (item_model.type == TURRET) {
      const f64 current_time = game_time::get_current_time();
      if(current_time < item.last_action_time + 1/item_model.fire_rate)
       continue;

      item.last_action_time = current_time;

      auto [has_enemy, enemy_id] = enemy::closest_enemy_in(item.position, 70);
      if (has_enemy)
        enemy::try_hit_enemy(enemy_id, item_model.damage);
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

} // namespace item
