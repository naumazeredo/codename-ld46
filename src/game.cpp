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

}

