#include "enemy.h"

#include "externs.h"

EnemyInfo enemy_info;

namespace enemy {

void debug_window() {
	ImGui::Text("Enemy");
	ImGui::SliderFloat("tx", &enemy_info.target.x, 0, SCREEN_WIDTH);
	ImGui::SliderFloat("ty", &enemy_info.target.y, 0, SCREEN_HEIGHT);
}

void setup() {
	debug::add_window(debug_window);
	enemy_info.target = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
	enemy_info.textures.push_back(render::load_image("assets/gfx/template-32x32-up.png"));
	Point pos = enemy_info.target;
	for(int i = 0; i < 1; ++i)
		spawn_enemy(pos, 10, rand() % 101);
}

void update() {
	f64 delta_time = game_time::get_frame_duration();
	for(Enemy& e : enemy_info.enemies) {
		Point diff = enemy_info.target - e.position;
		if(~diff < geometry::EPS)
			continue;
		if(~diff > 1)
			diff.normalize();
		diff.x *= delta_time * e.speed, diff.y *= delta_time * e.speed;
		e.position += diff;
	}
}

void spawn_enemy(Point position, u32 health, u32 speed)
{
	Enemy e;
	e.speed = speed;
	e.position = position;
	e.health = health;
	e.w = 32;
	e.h = 32;
	e.texture = enemy_info.textures[0]; 
	enemy_info.enemies.push_back(e);
}

void render()
{
	for(const Enemy &e : enemy_info.enemies)
		render::add_to_render(e.position.x - e.w / 2, e.position.y - e.h / 2, e.w, e.h, e.texture);
}

}