#include "enemy.h"

#include <string>
#include "externs.h"

EnemyInfo enemy_info;

namespace enemy {

void debug_window() {
  if (ImGui::TreeNode("Enemy")) {
    ImGui::Point("target", &enemy_info.target);

    if(ImGui::TreeNode("Enemies")) {
      for(auto &[enemy_id, enemy] : enemy_info.enemies) {
        ImGui::Text("%d: ", enemy_id);
        ImGui::SameLine();
        ImGui::SliderU32("health", &enemy.current_health, 0, 100); // @TODO(naum): use model health
      }

      ImGui::TreePop();
    }

    ImGui::TreePop();
  }
}

void setup() {
  EnemyModel tmp;

  tmp.speed = 10;
  tmp.health = 100;
  tmp.width = 32;
  tmp.height = 32;
  tmp.texture = TEX_ARROW_UP; 

  enemy_info.models.push_back(tmp);

  enemy_info.target = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

  geom::Point pos = enemy_info.target;
  for(int i = 0; i < 1; ++i)
    create_enemy(pos, 0);
}

EnemyModel& get_enemy_model(u32 id) {
  return enemy_info.models[enemy_info.enemies[id].model_id];
}

void update() {
  f64 delta_time = game_time::get_frame_duration();
  for(auto &[enemy_id, enemy] : enemy_info.enemies) {
    const auto &model = get_enemy_model(enemy_id);

    geom::Point diff = enemy_info.target - enemy.position;
    if(diff.abs() < geom::EPS)
      continue;
    if(diff.abs() > 1)
      diff.normalize();
    diff.x *= delta_time * model.speed, diff.y *= delta_time * model.speed;
    enemy.position += diff;
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
  enemy_info.enemies.erase(id);

  return true;
}

bool try_hit_enemy(u32 id, u32 damage) {
  if(!enemy_exists(id)) return false;

  if(enemy_info.enemies[id].current_health <= damage) {
    try_destroy_enemy(id);
    return true;
  }

  enemy_info.enemies[id].current_health -= damage;
  return true;
}

void create_enemy(geom::Point position, u32 model_id) {
  Enemy enemy;
  enemy.position = position;
  enemy.model_id = model_id;
  enemy.current_health = enemy_info.models[enemy.model_id].health; 
  enemy_info.enemies[++enemy_info.num_enemies] = enemy;
}

void render() {
  for(const auto &[enemy_id, enemy] : enemy_info.enemies) {
    const auto &model = get_enemy_model(enemy_id);
    render::add_to_render(
      enemy.position.x - model.width / 2,
      enemy.position.y - model.height / 2,
      model.width,
      model.height,
      model.texture
    );
  }
}

}
