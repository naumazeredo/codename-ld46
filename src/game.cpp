#include <imgui.h>
#include <string>
#include <iostream>
#include "game.h"
#include "externs.h"

GameInfo game_info;

namespace game {

void debug_window() {
  if (ImGui::TreeNode("Game")) {
    f64 health_percentage = king::get_king_health()/ KING_MAX_HEALTH;

    ImGui::Text("Health");
    ImGui::ProgressBar(health_percentage);

    std::string game_state_text;
    switch (game_info.current_state) {
      case RUNNING:
        game_state_text = "running";
        break;
      case GAME_OVER:
        game_state_text = "game over";
        break;
    }

    game_state_text = "Game State: " + game_state_text;
    ImGui::Text(game_state_text.c_str());

    ImGui::TreePop();
  }
}

void on_game_over_debug() {
  std::cout << "game over!" ;
}

void setup() {
  game_info.current_state = RUNNING;

  game_info.on_game_over.push_back(on_game_over_debug);

  game_info.bar_texture = TEX_BLANK;
  game_info.bar_h = 32;
  game_info.bar_w = 320;
  game_info.bar_position = geom::Point{};

  debug::add_window(debug_window);
}

void update() {
  if (king::get_king_health() < 0 ) {
    if (game_info.current_state != GAME_OVER) {
      game_info.current_state = GAME_OVER;
      for (auto callback : game_info.on_game_over){
        callback();
      }
    }
  }
}

void render() {
  float health_percentage = king::get_king_health() / KING_MAX_HEALTH;
  int w = game_info.bar_w * health_percentage;
  render::add_to_render(game_info.bar_position.x, game_info.bar_position.y, game_info.bar_w, game_info.bar_h, game_info.bar_texture);
  render::add_to_render(game_info.bar_position.x, game_info.bar_position.y, w, game_info.bar_h, game_info.bar_texture, Color{1,0,0,1});
}

}

