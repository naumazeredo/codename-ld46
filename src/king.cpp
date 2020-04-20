#include "king.h"

#include <algorithm>
#include "externs.h"

KingInfo king_info;

namespace king {

void debug_window() {
  if (ImGui::TreeNode("King")) {
    ImGui::InputDouble("health", &king_info.health);
    ImGui::InputFloat("x", &king_info.position.x, 1.0f, 10.0f);
    ImGui::InputFloat("y", &king_info.position.y, 1.0f, 10.0f);
    ImGui::InputInt("w", &king_info.w);
    ImGui::InputInt("h", &king_info.h);

    ImGui::TreePop();
  }
}
void setup() {
  king_info.health = KING_MAX_HEALTH;
  king_info.drop_model_ids = {2, 3, 8};
  king_info.w = 128;
  king_info.h = 128;

  king_info.position = {(float) SCREEN_WIDTH/2,
                        (float) SCREEN_HEIGHT/2};
  king_info.texture_pivot = {0, 0};

  enemy::set_target(king_info.position);

  auto idle_animation = animation::generate_animation_from_files(
    "assets/gfx/animations/goose",
    4
  );

  auto rect = geom::Rect{0, 0, (f32) king_info.w, (f32) king_info.h};

  std::vector<Animation> animations{idle_animation};
  AnimationSet set {animations};

  auto animation_instance_id = animation::add_animation_set(set);

  king_info.animation_instance_id = animation::add_animation_instance(animation_instance_id, rect);
}

void take_damage(f64 damage) {
  if(king_info.health <= damage) {
    king_info.health = 0;
    return;
  }
  king_info.health -= damage;

  for (auto callback : king_info.on_damage_taken){
    callback();
  }
}

geom::Point get_position() {
  return king_info.position;
}

void feed_king(f64 amount){
  king_info.health = std::min(king_info.health + amount, KING_MAX_HEALTH);

  const u64 drop_try = game::rand();
  if(drop_try%100 <=  STORE_DROP_RATE) {
    auto &drop_model_ids = king_info.drop_model_ids;
    auto &drop_model_id = drop_model_ids[drop_try%drop_model_ids.size()];
    item::create_item(drop_model_id, king_info.position + geom::Point(-10, -20));
  } 

  for (auto callback : king_info.on_feed_king){
    callback();
  }
}


u32 get_health() {
  return king_info.health;
}

void update() {
  take_damage(game_time::get_frame_duration()*KING_HUNGER_RATE);
  animation::set_animation_instance_pos(
      king_info.animation_instance_id,
      king_info.position.x,
      king_info.position.y,
      (f32) king_info.w,
      (f32) king_info.h,
      king_info.position.y
  );
}

}
