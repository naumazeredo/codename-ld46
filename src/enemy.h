#pragma once

#include <vector>
#include <map>
#include <utility>

#include "texture.h"
#include "types.h"
#include "geom.h"

struct EnemyModel {
  u32 animation_set_id;

  u32 w, h;

  u32 health;
  u32 damage;
  u32 speed;
  std::vector<u32> drop_model_ids;
};

struct Enemy {
  u32 model_id;
  geom::Point position;
  u32 current_health;
  u32 animation_instance_id;
};

struct EnemyInfo {
  u32 num_enemies;
  std::vector<EnemyModel> models;
  std::map<u32, Enemy> enemies;
  geom::Point target;
};

namespace enemy {

void setup();
void render();
void update();
void debug_window();

void create_enemy(geom::Point position, u32 model_id);
bool try_hit_enemy(u32 id, u32 damage);
bool try_destroy_enemy(u32 id);
void set_target(geom::Point target);

std::pair<bool, u32> closest_enemy_in(geom::Point position, f64 range);

}
