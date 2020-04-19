#pragma once

#include <vector>
#include <map>

#include "types.h"
#include "geom.h"

struct Enemy {
  geom::Point position;
  u32 w, h, health;
  u32 texture, speed;
};

struct EnemyInfo {
  u32 num_enemies;
  std::vector<u32> textures;
  std::map<u32, Enemy> enemies;
  geom::Point target;
};

namespace enemy {

void setup();
void render();
void update();
void spawn_enemy(geom::Point position, u32 health, u32 speed);
u32 closest_enemy_in(geom::Point position, f64 range);
bool hit_enemy(u32 id, u32 damage);

}
