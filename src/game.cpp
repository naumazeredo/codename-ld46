#include <imgui.h>
#include <string>
#include <iostream>
#include "game.h"
#include "externs.h"

GameInfo game_info;

namespace game {

void debug_window(){
  ImGui::Text("Game");

  f64 health_percentage = game_info.king_health / KING_MAX_HEALTH;

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
}

void on_game_over_debug() {
  std::cout << "game over!" ;
}

void setup() {
  game_info.king_health = KING_MAX_HEALTH;
  game_info.current_state = RUNNING;

  game_info.on_game_over.push_back(on_game_over_debug);

  game_info.bar_texture = render::load_image("assets/gfx/blank.png");
  game_info.bar_h = 32;
  game_info.bar_w = 320;
  game_info.bar_position = Point{};

  debug::add_window(debug_window);
}

void update() {
  if (game_info.king_health < 0 ) {
    if (game_info.current_state != GAME_OVER) {
      game_info.current_state = GAME_OVER;
      for (auto callback : game_info.on_game_over){
        callback();
      }
    }
  } else {
    game_info.king_health -= KING_HUNGER_RATE;
  }
}

void take_damage(f64 damage) {
  game_info.king_health -= damage;

    for (auto callback : game_info.on_damage_taken){
      callback();
    }
}

void render() {
  float health_percentage = game_info.king_health / KING_MAX_HEALTH;
  int w = game_info.bar_w * health_percentage;
  render::add_to_render(game_info.bar_position.x, game_info.bar_position.y, game_info.bar_w, game_info.bar_h, game_info.bar_texture);
  render::add_to_render(game_info.bar_position.x, game_info.bar_position.y, w, game_info.bar_h, game_info.bar_texture, Color{1,0,0,0});
}

}

