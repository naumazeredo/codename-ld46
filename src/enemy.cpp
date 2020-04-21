#include "enemy.h"

#include <string>
#include <vector>

#include "externs.h"

EnemyInfo enemy_info;

namespace enemy {

void debug_window() {
  if (ImGui::TreeNode("Enemies")) {
    if (ImGui::TreeNode("Models")) {
      int model_id = 0;
      for (auto& model : enemy_info.models) {
        ImGui::PushID(&model);

        ImGui::Text("Model %d", model_id++);

        //ImGui::SliderContainer("texture", &model.texture, render_info.texture);
        //ImGui::Text("texture: %u", (u32)model.texture);
        ImGui::SliderU32("w", &model.w, 0, 128);
        ImGui::SliderU32("h", &model.h, 0, 128);
        ImGui::SliderU32("health", &model.health, 0, 128);
        ImGui::SliderU32("damage", &model.damage, 0, 128);
        ImGui::SliderU32("speed", &model.speed, 0, 128);

        //std::vector<u32> drop_model_ids;

        ImGui::PopID();
      }

      ImGui::TreePop();
    }

    if (ImGui::TreeNode("Enemies")) {
      for (auto &[id, enemy] : enemy_info.enemies) {
        std::string label = "Enemy: " + std::to_string(id);

        if (ImGui::TreeNode(label.c_str())) {
          ImGui::SliderContainer("model", &enemy.model_id, enemy_info.models);
          ImGui::Point("position", &enemy.position);
          ImGui::SliderU32("current_health", &enemy.current_health, 0, 128);

          ImGui::TreePop();
        }
      }

      ImGui::TreePop();
    }

    ImGui::TreePop();
  }
}

void setup() {
  EnemyModel tmp;

  auto fox_animation = animation::generate_animation_from_files(
    "assets/gfx/animations/fox",
    2
  );

  tmp.w = 48;
  tmp.h = 32;

  auto rect = geom::Rect{0, 0, (f32) tmp.w, (f32) tmp.h};
  std::vector<Animation> animations{fox_animation};
  AnimationSet set {animations};

  auto animation_set_id = animation::add_animation_set(set);

  tmp.speed = 20;
  tmp.animation_set_id = animation_set_id;
  tmp.health = 100;
  tmp.damage = 15;
  tmp.drop_model_ids = {0, 1};

  enemy_info.models.push_back(tmp);

  enemy_info.target = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
}

void cleanup() {
  enemy_info.enemies.clear();
  enemy_info.models.clear();
  enemy_info.num_enemies = 0;
}

EnemyModel& get_enemy_model(u32 id) {
  return enemy_info.models[enemy_info.enemies[id].model_id];
}

bool try_attack_king(u32 enemy_id) {
  const auto &enemy = enemy_info.enemies[enemy_id];
  const auto &enemy_model = get_enemy_model(enemy_id);

  if((king::get_position() - enemy.position).abs() < 20) {
    king::take_damage(enemy_model.damage);
    return true;
  }

  return false;
}

void update_render_info(EnemyModel model, Enemy enemy) {
  bool flip_horizontal = enemy_info.target.x - enemy.position.x < 0;
  if (enemy.animation_instance_id != -1) {
    animation::set_animation_instance_pos(
      enemy.animation_instance_id,
      enemy.position.x,
      enemy.position.y,
      (f32) model.w,
      (f32) model.h,
      enemy.position.y,
      flip_horizontal
    );
  }
}

void update() {
  f64 delta_time = game_time::get_frame_duration();
  std::vector<u32> enemy_ids;

  for(auto &[enemy_id, enemy] : enemy_info.enemies) {
    enemy_ids.push_back(enemy_id);

    const auto &model = get_enemy_model(enemy_id);

    update_render_info(model, enemy);

    geom::Point diff = enemy_info.target - enemy.position;
    if(diff.abs() < geom::EPS)
      continue;
    if(diff.abs() > 1)
      diff.normalize();
    diff.x *= delta_time * model.speed, diff.y *= delta_time * model.speed;
    enemy.position += diff;
  }

  for(const auto &enemy_id : enemy_ids) {
    if(try_attack_king(enemy_id)) {
      try_destroy_enemy(enemy_id);
    }
  }

}

std::pair<bool, u32> closest_enemy_in(geom::Point position, f64 range) {
  u32 enemy_id = -1;
  f64 smallest_distance = range + geom::EPS;

  bool found_enemy = false;
  for(const auto &[e_id, e] : enemy_info.enemies) {
    f64 distance = (e.position - position).abs();
    if(distance < smallest_distance) {
      enemy_id = e_id;
      smallest_distance = distance;

      found_enemy = true;
    }
  }

  return { found_enemy, enemy_id };
}

bool enemy_exists(u32 id) {
  if(enemy_info.enemies.find(id) == enemy_info.enemies.end()) return false;
  return true;
}

bool try_destroy_enemy(u32 id) {
  if(!enemy_exists(id)) return false;

  auto animation_instance_id = enemy_info.enemies[id].animation_instance_id;
  if (animation_instance_id != -1)
    animation::destroy_instance(animation_instance_id);

  enemy_info.enemies.erase(id);

  return true;
}

bool try_hit_enemy(u32 id, u32 damage) {
  if(!enemy_exists(id)) return false;

  auto &enemy = enemy_info.enemies[id];

  if(enemy_info.enemies[id].current_health <= damage) {
    const auto &enemy_model = get_enemy_model(id);
    const auto &drop_model_ids = enemy_model.drop_model_ids;

    if ((rand() % 100) < 50) {
      if(drop_model_ids.size() > 0) {
        u32 drop_model_id = drop_model_ids[game_info.rand()%drop_model_ids.size()];
        item::create_item(drop_model_id, enemy.position);
      }
    }

    try_destroy_enemy(id);
    return true;
  }

  enemy.current_health -= damage;
  return true;
}

void set_target(geom::Point target) {
  enemy_info.target = target;
}

void create_enemy(geom::Point position, u32 model_id) {
  Enemy enemy;
  enemy.position = position;
  enemy.model_id = model_id;
  auto& model = enemy_info.models[enemy.model_id]; 
  enemy.current_health = model.health;

  auto animation_set_id = model.animation_set_id;
  if (animation_set_id != -1) {
    auto rect = geom::Rect { 0.0f, 0.0f, (f32)model.w, (f32)model.h };
    enemy.animation_instance_id = animation::add_animation_instance(animation_set_id, rect);
  }

  enemy_info.enemies[++enemy_info.num_enemies] = enemy;
}

void render() {
  for (const auto &[enemy_id, enemy] : enemy_info.enemies) {
    const auto &model = get_enemy_model(enemy_id);
    f64 health_percentage = enemy.current_health / (f64)model.health;
    int w = game_info.bar_w * health_percentage;

    geom::Point bar_position = enemy.position + geom::Point(-20, 35);
    s32 bar_w = 40;
    s32 bar_h = 6;

    render::add_to_render(
      bar_position.x,
      bar_position.y,
      bar_w,
      bar_h,
      TextureCode::TEX_BLANK,
      enemy.position.y + 1
    );
    render::add_to_render(
      bar_position.x,
      bar_position.y,
      bar_w*health_percentage,
      bar_h,
      TextureCode::TEX_BLANK,
      enemy.position.y,
      Color{1, 0, 0, 1}
    );
  }
}

} // namespace enemy
