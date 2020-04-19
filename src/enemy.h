#pragma once

#include <vector>

#include "types.h"
#include "geom.h"

struct Enemy {
  geom::Point position;
  u32 w, h, health;
  u32 texture, speed;
};

struct EnemyInfo {
  std::vector<u32> textures;
  std::vector<Enemy> enemies;
  geom::Point target;
};

namespace enemy {

void setup();
void render();
void update();
void spawn_enemy(geom::Point position, u32 health, u32 speed);

}
