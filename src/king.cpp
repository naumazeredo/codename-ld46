#include "king.h"

#include "externs.h"

KingInfo king_info;

namespace king {

void debug_window() {
  if (ImGui::TreeNode("King")) {
    ImGui::InputFloat("x", &king_info.position.x, 1.0f, 10.0f);
    ImGui::InputFloat("y", &king_info.position.y, 1.0f, 10.0f);
    ImGui::InputInt("w", &king_info.w);
    ImGui::InputInt("h", &king_info.h);

    ImGui::TreePop();
  }
}
void setup() {
  king_info.king_health = KING_MAX_HEALTH;
  king_info.w = 100;
  king_info.h = 150;
  king_info.position = {(float) SCREEN_WIDTH/2 - king_info.w/2,
    (float) SCREEN_HEIGHT/2 - king_info.h/2};
  king_info.texture = TEX_ARROW_DOWN;

  enemy::set_target(king_info.position);

  debug::add_window(debug_window);
}

void take_damage(f64 damage) {
  if(king_info.king_health <= damage) {
    king_info.king_health = 0;
    return;
  }
  king_info.king_health -= damage;

  for (auto callback : king_info.on_damage_taken){
    callback();
  }
}

geom::Point get_position() {
  return king_info.position;
}

void feed_king(f64 amount){
  king_info.king_health = min(king_info.king_health + amount, KING_MAX_HEALTH);

  for (auto callback : king_info.on_feed_king){
    callback();
  }
}


u32 get_king_health() {
  return king_info.king_health;
}

void render() {
  render::add_to_render(king_info.position.x - king_info.w / 2, king_info.position.y, king_info.w, king_info.h, king_info.texture);
}

}
