#include "player.h"

#include "externs.h"

PlayerInfo player_info;

namespace player {

void debug_window() {
  ImGui::Text("Player");

  ImGui::InputFloat("x", &player_info.position.x, 1.0f, 10.0f);
  ImGui::InputFloat("y", &player_info.position.y, 1.0f, 10.0f);
  ImGui::InputInt("w", &player_info.w);
  ImGui::InputInt("h", &player_info.h);
  //ImGui:: enum
  ImGui::InputInt("speed", &player_info.speed);
}

void setup() {
  player_info.position = {(float) SCREEN_WIDTH/2 - player_info.w/2,
                          (float) SCREEN_HEIGHT/2 - player_info.h/2};
  player_info.w = 50;
  player_info.h = 100;
  player_info.direction = DOWN;
  player_info.speed = 100;

  player_info.textures[0] = render::load_image("assets/gfx/template-32x32-up.png");
  player_info.textures[1] = render::load_image("assets/gfx/template-32x32-down.png");
  player_info.textures[2] = render::load_image("assets/gfx/template-32x32-left.png");
  player_info.textures[3] = render::load_image("assets/gfx/template-32x32-right.png");


  { //teste
    Point position = player_info.position;
    position.y += player_info.h/2;

    player_info.item = item::create_item(0, position);
  }

  // add debug window
  debug::add_window(debug_window);
}

void render() {
  render::add_to_render(player_info.position.x - player_info.w / 2, player_info.position.y - player_info.h / 2, player_info.w, player_info.h, player_info.textures[player_info.direction]);
}

void update() {
  Point position = player_info.position;
  position.y += player_info.h/2;
  item::update_position(player_info.item, position);
}

}
