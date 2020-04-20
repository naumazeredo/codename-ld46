#include "item.h"

#include <string>
#include <stdio.h>
#include <vector>

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

        //ImGui::SliderContainer("texture", &model.texture, render_info.texture);
        ImGui::Text("texture: %u", (u32)model.texture);
        ImGui::SliderU32("w", &model.w, 0, 128);
        ImGui::SliderU32("h", &model.h, 0, 128);

        switch (model.type) {
          case ItemType::CANDY:
            ImGui::SliderU8("hunger count", &model.hunger_count, 0, 255);
          break;
          case ItemType::TRAP:
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
          ImGui::SliderContainer("model", &item.model_id, item_info.models);
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

  tmp.type = ItemType::CANDY;
  tmp.texture = TextureCode::TEX_CANDY;
  tmp.shadow = TextureCode::INVALID;
  tmp.texture_pivot_x = -15;
  tmp.texture_pivot_y = -15;
  tmp.animation_set_id = -1;
  tmp.w = 30;
  tmp.h = 30;
  tmp.hunger_count = 5;
  item_info.models.push_back(tmp);

  tmp.type = ItemType::MONEY;
  tmp.texture = TextureCode::TEX_MONEY;
  tmp.shadow = TextureCode::INVALID;
  tmp.texture_pivot_x = -15;
  tmp.texture_pivot_y = -15;
  tmp.w = 30;
  tmp.h = 30;
  item_info.models.push_back(tmp);

  tmp.type = ItemType::SHOP;
  tmp.texture = TextureCode::TEX_TRAP_SHOP;
  tmp.shadow = TextureCode::INVALID;
  tmp.texture_pivot_x = -20;
  tmp.texture_pivot_y = -15;
  tmp.animation_set_id = -1;
  tmp.w = 40;
  tmp.h = 30;
  tmp.shop_model_id = 0;
  item_info.models.push_back(tmp);

  tmp.type = ItemType::FACTORY;
  tmp.texture = TextureCode::TEX_FACTORY;
  tmp.shadow = TextureCode::INVALID;
  tmp.texture_pivot_x = -20;
  tmp.texture_pivot_y = -15;
  tmp.animation_set_id = -1;
  tmp.w = 40;
  tmp.h = 30;
  tmp.shop_model_id = 2;
  item_info.models.push_back(tmp);

  tmp.type = ItemType::UNPICKABLE;
  tmp.texture = TextureCode::TEX_LOG;
  tmp.shadow = TextureCode::TEX_LOG_SHADOW;
  tmp.animation_set_id = -1;
  tmp.w = 64;
  tmp.h = 64;
  tmp.texture_pivot_x = -(32);
  tmp.texture_pivot_y = -(5);
  item_info.models.push_back(tmp);

  create_logs_line(item_info.models.size() - 1, {939, 456}, 1, 1);
  create_logs_line(item_info.models.size() - 1, {935, 230}, 1, -1);
  create_logs_line(item_info.models.size() - 1, {362, 456}, -1, 1);
  create_logs_line(item_info.models.size() - 1, {362, 230}, -1, -1);

  tmp.type = ItemType::UNPICKABLE;
  tmp.texture = TextureCode::TEX_KING_ROCK;
  tmp.shadow = TextureCode::TEX_KING_ROCK_SHADOW;
  tmp.animation_set_id = -1;
  tmp.w = 180;
  tmp.h = 100;
  tmp.texture_pivot_x = -((f32)tmp.w/2);
  tmp.texture_pivot_y = -((f32)tmp.h/2 + 5);
  item_info.models.push_back(tmp);

  create_item(item_info.models.size()-1,
              {(float) SCREEN_WIDTH/2, (float) SCREEN_HEIGHT/2 + 2});

  auto spike_animation = animation::generate_animation_from_files(
    "assets/gfx/animations/spike",
    2
  );

  tmp.w = 32;
  tmp.h = 32;

  auto rect = geom::Rect{0, 0, (f32) tmp.w, (f32) tmp.h};
  std::vector<Animation> animations{spike_animation};
  AnimationSet set {animations};

  auto animation_set_id = animation::add_animation_set(set);

  tmp.type = ItemType::TRAP;
  tmp.animation_set_id = animation_set_id;

  tmp.shadow = TextureCode::INVALID;
  tmp.texture_pivot_x = 16;
  tmp.texture_pivot_y = 16;
  tmp.damage = 1;
  tmp.action_rate = 5;
  tmp.action_range = 20;
  tmp.damage = 10;
  item_info.models.push_back(tmp);

  auto turret_animation = animation::generate_animation_from_files(
    "assets/gfx/animations/turret_shooting",
    3
  );

  tmp.w = 32;
  tmp.h = 32;

  rect = geom::Rect{0, 0, (f32) tmp.w, (f32) tmp.h};
  animations = {turret_animation};
  set = {animations};

  animation_set_id = animation::add_animation_set(set);

  tmp.type = ItemType::TURRET;
  tmp.animation_set_id = animation_set_id;
  tmp.shadow = TextureCode::INVALID;
  tmp.damage = 10;
  tmp.action_rate = 2; // Shots Per Second
  tmp.action_range = 100;
  item_info.models.push_back(tmp);

  tmp.type = ItemType::SHOP;
  tmp.texture = TextureCode::TEX_TURRET_SHOP;
  tmp.shadow = TextureCode::INVALID;
  tmp.texture_pivot_x = -20;
  tmp.texture_pivot_y = -15;
  tmp.animation_set_id = -1;
  tmp.w = 40;
  tmp.h = 30;
  tmp.shop_model_id = 1;
  item_info.models.push_back(tmp);
}

std::tuple<bool, ItemModel> get_model_by_item_id(u32 item_id) {
  auto [item_found, item] = get_item_by_id(item_id);
  if (!item_found) {
    printf("[error] item not found: %d\n", item_id);
    return {false, {}};
  }

  u32 item_model_id = item.model_id;
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
  auto [_, item] = item::get_item_by_id(id);

  auto animation_instance_id = item.animation_instance_id;
  if (animation_instance_id != -1)
    animation::destroy_instance(animation_instance_id);

  item_info.items.erase(id);
  return true;
}

void update_render_info(u32 id) {
  Item &item = item_info.items[id];
  auto& item_model = item_info.models[item.model_id];

  s32 z = 9999;
  if (item.being_held) {
    z = player_info.position.y;
  } else {
    if (item_model.type == ItemType::TRAP) z = 9999;
    else z = item.position.y;
  }

  bool flip_horizontal = item.position.x < SCREEN_WIDTH/2; // TODO: refactor to is_on_left_side();

  if (item.animation_instance_id != -1) {
    animation::set_animation_instance_pos(
      item.animation_instance_id,
      item.position.x,
      item.position.y,
      (f32) item_model.w,
      (f32) item_model.h,
      z,
      flip_horizontal
    );
  }
}

bool update_position(u32 id, geom::Point position) {
  if (!item_exists(id)) return false;

  Item &item = item_info.items[id];

  item.position = position;

  update_render_info(id);

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
  for(auto &[id, item]: item_info.items) {
    const auto &item_model = item_info.models[item.model_id];
    if(item.being_held && item_model.type == ItemType::TRAP) continue;

    const f64 current_time = game_time::get_time();
    if (item_model.type == ItemType::TURRET || item_model.type == ItemType::TRAP) {
      if(current_time < item.last_action_time + 1/item_model.action_rate)
       continue;

      item.last_action_time = current_time;

      auto [has_enemy, enemy_id] = enemy::closest_enemy_in(item.position, item_model.action_range);
      if (has_enemy)
        enemy::try_hit_enemy(enemy_id, item_model.damage);
    }

    update_render_info(id);
  }
}

float dist_to_item(geom::Point position, u32 item_id) {
  return (position - item_info.items[item_id].position).abs();
}

u32 closest_item(geom::Point position) {
  if(item_info.items.empty())
    return 0;

  u32 ans = 0;
  float dist = 1e12;

  for (auto [id, item]: item_info.items) {
    float d = dist_to_item(position, id);

    auto [_, model] = get_model_by_item_id(id);

    if (model.type != ItemType::UNPICKABLE and d < dist) {
      dist = d;
      ans = id;
    }
  }

  return ans;
}

u32 create_item(u32 model_id, geom::Point position) {
  u32 id = ++item_info.num_item;
  Item item;

  item.id = id;
  item.model_id = model_id;
  item.last_action_time = game_time::get_real_time();
  item.being_held = false;

  item.position = position;

  auto& model = item_info.models[model_id];
  auto animation_set_id = model.animation_set_id;
  if (animation_set_id != -1) {
    auto rect = geom::Rect { 0.0f, 0.0f, (f32)model.w, (f32)model.h };
    item.animation_instance_id = animation::add_animation_instance(animation_set_id, rect);
  } else {
    item.animation_instance_id = -1;
  }

  item_info.items[id] = item;


  return id;
}

void create_logs_line(u32 model_id, geom::Point base, int x_dir, int y_dir) {

  geom::Polygon bound;

  x_dir /= abs(x_dir);
  y_dir /= abs(y_dir);
  std::vector<geom::Point> tmp;

  tmp.push_back({28, 10});
  tmp.push_back({60, 18});
  tmp.push_back({90, 29});
  tmp.push_back({126, 33});
  tmp.push_back({161, 46});
  tmp.push_back({192, 49});

  for(auto &p: tmp) p.y *= y_dir;
  for(auto &p: tmp) p.x *= x_dir;

  create_item(model_id, base);

  bound.push_back({base.x, base.y + y_dir*200});
  bound.push_back(base);

  for(auto p: tmp) create_item(model_id, base + p);

  base += tmp.back();

  for(auto p: tmp) create_item(model_id, base + p);

  auto aux = base + tmp.back();
  bound.push_back(aux);
  bound.push_back({aux.x, aux.y + y_dir*200});

  if(y_dir != x_dir) reverse(bound.begin(), bound.end());
  bound_info.bounds.push_back(bound);

  for(auto &p: tmp) p.y = -p.y;
  for(auto p: tmp) create_item(model_id, base + p);
}

void render() {
  for (auto [id, item]: item_info.items) {
    auto& model = item_info.models[item.model_id];
    if (item.animation_instance_id == -1) {
      auto z = (item.being_held ? player_info.position.y : item.position.y);

      render::add_to_render(item.position.x + model.texture_pivot_x, item.position.y + model.texture_pivot_y, model.w, model.h, model.texture, z);

      if (model.shadow != TextureCode::INVALID) {
        render::add_to_render(item.position.x + model.texture_pivot_x, item.position.y + model.texture_pivot_y, model.w, model.h, model.shadow, 99999);
      }
    }
  }
}

} // namespace item
