#include "player.h"

#include "externs.h"

PlayerInfo player_info;

namespace player {

void debug_window() {
  if (ImGui::CollapsingHeader("Player")) {
    ImGui::InputFloat("x", &player_info.position.x, 1.0f, 10.0f);
    ImGui::InputFloat("y", &player_info.position.y, 1.0f, 10.0f);
    ImGui::InputInt("w", &player_info.w);
    ImGui::InputInt("h", &player_info.h);
    //ImGui:: enum
    ImGui::InputInt("speed", &player_info.speed);
  }
}

void setup() {
  player_info.position = {(float) SCREEN_WIDTH/2 - player_info.w/2,
                          (float) SCREEN_HEIGHT/2 - player_info.h/2};
  player_info.w = 50;
  player_info.h = 100;
  player_info.direction = DOWN;
  player_info.speed = 100;

  player_info.item_position = {0, (float) player_info.h};
  player_info.item_max_dist = 40;

  player_info.textures[0] = render::load_image("assets/gfx/template-32x32-up.png");
  player_info.textures[1] = render::load_image("assets/gfx/template-32x32-down.png");
  player_info.textures[2] = render::load_image("assets/gfx/template-32x32-left.png");
  player_info.textures[3] = render::load_image("assets/gfx/template-32x32-right.png");


  { //teste
    geom::Point position = player_info.position;
    position.y += player_info.h/2;

    player_info.item = item::create_item(2, position);
  }

  // add debug window
  debug::add_window(debug_window);
}

void render() {
  render::add_to_render(player_info.position.x - player_info.w / 2, player_info.position.y, player_info.w, player_info.h, player_info.textures[player_info.direction]);
}

void update() {
  item::update_position(player_info.item, player_info.position + player_info.item_position);
}

void drop_item() {
  item::update_position(player_info.item, player_info.position);
  player_info.item = 0;
}

void item_interaction() {
  if(item::exists_item(player_info.item)) {
    drop_item();
  }
  else {
    geom::Point position = player_info.position;

    u32 item = item::closest_item(position);
    if(item::dist_to_item(position, item) < player_info.item_max_dist) {
      player_info.item = item;
    }
  }
}

}
