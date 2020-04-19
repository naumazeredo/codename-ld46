#include "enemy.h"

#include <string>
#include "externs.h"

EnemyInfo enemy_info;

namespace enemy {

void debug_window() {
  if (ImGui::CollapsingHeader("Enemy")) {
    ImGui::Indent(10);
    ImGui::SliderFloat("tx", &enemy_info.target.x, 0, SCREEN_WIDTH);
    ImGui::SliderFloat("ty", &enemy_info.target.y, 0, SCREEN_HEIGHT);
    if(ImGui::CollapsingHeader("Enemies")) {
      for(auto &[enemy_id, enemy] : enemy_info.enemies) {
        std::string text = std::to_string(enemy_id) + " - Heath: " + std::to_string(enemy.health);
        ImGui::Text(text.c_str());
      }
    }
    ImGui::Indent(-10.0);
  }
}

void setup() {
  debug::add_window(debug_window);
  enemy_info.target = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
  enemy_info.textures.push_back(render::load_image("assets/gfx/template-32x32-up.png"));
  geom::Point pos = enemy_info.target;
  for(int i = 0; i < 1; ++i)
    spawn_enemy(pos, 10, rand() % 101);
}

void update() {
  f64 delta_time = game_time::get_frame_duration();
  for(auto &[e_id, e] : enemy_info.enemies) {
    geom::Point diff = enemy_info.target - e.position;
    if(diff.abs2() < geom::EPS)
      continue;
    if(diff.abs2() > 1)
      diff.normalize();
    diff.x *= delta_time * e.speed, diff.y *= delta_time * e.speed;
    e.position += diff;
  }
}

u32 closest_enemy_in(geom::Point position, f64 range) {
  u32 enemy_id = -1;
  f64 smallest_distance = range + geom::EPS;

  for(const auto &[e_id, e] : enemy_info.enemies) {
    f64 distance = (e.position - position).abs2();
    if(distance < smallest_distance) {
      enemy_id = e_id;
      smallest_distance = distance;
    }
  }

  return enemy_id;
}

bool hit_enemy(u32 id, u32 damage) {
  if(enemy_info.enemies.find(id) == enemy_info.enemies.end()) return false;

  enemy_info.enemies[id].health -= damage;
  return true;
}

void spawn_enemy(geom::Point position, u32 health, u32 speed) {
  Enemy e;
  e.speed = speed;
  e.position = position;
  e.health = health;
  e.w = 32;
  e.h = 32;
  e.texture = enemy_info.textures[0]; 
  enemy_info.enemies[++enemy_info.num_enemies] = e;
}

void render() {
  for(const auto &[e_id, e] : enemy_info.enemies) {
    render::add_to_render(e.position.x - e.w / 2, e.position.y - e.h / 2, e.w, e.h, e.texture);
  }
}

}
