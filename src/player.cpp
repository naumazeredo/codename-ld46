#include "player.h"

#include "render.h"
#include "types.h"
#include "externs.h"

PlayerInfo player_info;

namespace player {

void setup() {
  player_info.w = 50;
  player_info.h = 100;
  player_info.position = {(float) SCREEN_WIDTH/2 - player_info.w/2,
                          (float) SCREEN_HEIGHT/2 - player_info.h/2};
  player_info.direction = DOWN;
  player_info.speed = 100;

  player_info.textures[0] = render::load_image("assets/gfx/template-32x32-up.png");
  player_info.textures[1] = render::load_image("assets/gfx/template-32x32-down.png");
  player_info.textures[2] = render::load_image("assets/gfx/template-32x32-left.png");
  player_info.textures[3] = render::load_image("assets/gfx/template-32x32-right.png");

  render::add_to_render(player_info.position.x, player_info.position.y, player_info.w, player_info.h, player_info.textures[player_info.direction]);
}

void update() {
  render::add_to_render(player_info.position.x, player_info.position.y, player_info.w, player_info.h, player_info.textures[player_info.direction]);
}

}
