#pragma once
#include "types.h"
#include <vector>

struct Enemy {
	Point position;
	u32 w, h, health;
	u32 texture, speed;
};

struct EnemyInfo {
	std::vector<u32> textures;
	std::vector<Enemy> enemies;
	Point target;
};

namespace enemy {
	void setup();
	void render();
	void update();
	void spawn_enemy(Point position, u32 health, u32 speed);
}