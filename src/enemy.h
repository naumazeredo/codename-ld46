#pragma once

#include <vector>
#include <map>
#include <utility>

#include "types.h"
#include "geom.h"

struct EnemyModel {
  u32 texture;
  u32 width, height;

  u32 health;
  u32 speed;
};

struct Enemy {
  u32 model_id;
  geom::Point position;
  u32 current_health;
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

std::pair<bool, u32> closest_enemy_in(geom::Point position, f64 range);

}
