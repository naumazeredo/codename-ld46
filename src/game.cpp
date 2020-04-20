#include "game.h"

#include <cstdio>
#include <string>
#include <chrono>
#include <ctime>
#include <climits>

#include <imgui.h>

#include "externs.h"

GameInfo game_info;

namespace game {

void debug_window() {
  if (ImGui::TreeNode("Game")) {
    f64 health_percentage = king::get_health()/ KING_MAX_HEALTH;

    ImGui::Text("Health");
    ImGui::ProgressBar(health_percentage);

    std::string game_state_text;
    switch (game_info.current_state) {
      case GameState::RUNNING:
        game_state_text = "running";
      break;
      case GameState::GAME_OVER:
        game_state_text = "game over";
      break;
    }

    game_state_text = "Game State: " + game_state_text;
    ImGui::Text(game_state_text.c_str());

    ImGui::InputDouble("Wave delay", &game_info.wave_time);
    ImGui::Text("Wave in %fs", game_info.wave_remaining_time);

    ImGui::TreePop();
  }
}

void on_game_over_debug() {
  printf("game over!");
}

void setup() {
  game_info.rand = std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count());
  game_info.last_wave_cnt = 0;
  game_info.wave_enemy_increase = 1;
  game_info.wave_time = 2.0f;
  game_info.wave_remaining_time = game_info.wave_time;

  game_info.spawn_pool.push_back({0, SCREEN_HEIGHT / 2.0f - 25, 50, 50});
  game_info.spawn_pool.push_back({SCREEN_WIDTH - 50, SCREEN_HEIGHT / 2.0f - 25, 50, 50});
  //game_info.spawn_pool.push_back({SCREEN_WIDTH / 2.0f - 25, 0, 50, 50});
  //game_info.spawn_pool.push_back({SCREEN_WIDTH / 2.0f - 25, SCREEN_HEIGHT - 50, 50, 50});

  game_info.current_state = GameState::RUNNING;

  game_info.on_game_over.push_back(on_game_over_debug);

  game_info.bar_texture = TextureCode::TEX_BLANK;
  game_info.bar_h = 32;
  game_info.bar_w = 320;
  game_info.bar_position = geom::Point{SCREEN_WIDTH/2 - (f32) game_info.bar_w/2, SCREEN_HEIGHT - (f32) game_info.bar_h};
}

void update() {
  f64 delta_time = game_time::get_frame_duration();
  f64 cur_time   = game_time::get_time();

  game_info.wave_remaining_time -= delta_time;

  if (enemy_info.enemies.size() != 0)
    game_info.wave_remaining_time = game_info.wave_time;

  if (game_info.wave_remaining_time < 0) {
    game_info.last_wave_cnt += game_info.wave_enemy_increase;
    spawn_wave(game_info.last_wave_cnt);
  }

  if (king::get_health() <= 0 ) {
    if (game_info.current_state != GameState::GAME_OVER) {
      game_info.current_state = GameState::GAME_OVER;
      for (auto callback : game_info.on_game_over){
        callback();
      }
    }
  }
}

void spawn_wave(u32 enemy_count) {
  for(int i = 0; i < enemy_count; ++i) {
    geom::Rect &spawn_area = game_info.spawn_pool[game_info.rand() % game_info.spawn_pool.size()];
    geom::Point pos;
    pos.x = spawn_area.x + game_info.rand() * spawn_area.w / (LLONG_MAX);
    pos.y = spawn_area.y + game_info.rand() * spawn_area.h / (LLONG_MAX);
    enemy::create_enemy(pos, 0);
  }
}

u64 rand() {
  return game_info.rand();
}

void render() {
  float health_percentage = king::get_health() / KING_MAX_HEALTH;
  int w = game_info.bar_w * health_percentage;
  render::add_to_render(game_info.bar_position.x, game_info.bar_position.y, game_info.bar_w, game_info.bar_h, game_info.bar_texture, 20000);
  render::add_to_render(game_info.bar_position.x, game_info.bar_position.y, w, game_info.bar_h, game_info.bar_texture, 10000, Color{1,0,0,1});
}

}

