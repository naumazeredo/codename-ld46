#include "player.h"

#include "externs.h"

PlayerInfo player_info;

namespace player {

void debug_window() {
  if (ImGui::TreeNode("Player")) {
    ImGui::InputFloat("x", &player_info.position.x, 1.0f, 10.0f);
    ImGui::InputFloat("y", &player_info.position.y, 1.0f, 10.0f);
    ImGui::InputInt("w", &player_info.w);
    ImGui::InputInt("h", &player_info.h);
    //ImGui:: enum
    ImGui::InputInt("speed", &player_info.speed);
    ImGui::Text("Item: %u", &player_info.item);

    ImGui::TreePop();
  }
}

void setup() {
  player_info.w = 50;
  player_info.h = 100;
  player_info.position = {(float) SCREEN_WIDTH/3 - player_info.w/2,
                          (float) SCREEN_HEIGHT/3 - player_info.h/2};
  player_info.direction = DOWN;
  player_info.speed = 100;

  player_info.item_position = {0, (float) player_info.h};
  player_info.item_max_dist = 40;

  player_info.textures[0] = TEX_ARROW_UP;
  player_info.textures[1] = TEX_ARROW_DOWN;
  player_info.textures[2] = TEX_ARROW_LEFT;
  player_info.textures[3] = TEX_ARROW_RIGHT;

  // add debug window
  debug::add_window(debug_window);
}

void render() {
  render::add_to_render(player_info.position.x - player_info.w / 2, player_info.position.y, player_info.w, player_info.h, player_info.textures[player_info.direction]);
}

void update() {
  if(player_info.item)
    item::update_position(player_info.item, player_info.position + player_info.item_position);
}

void drop_item() {
  if(!item::drop_item(player_info.item)) return;

  item::update_position(player_info.item, player_info.position);
  player_info.item = 0;
}

void use_item() {
  for(auto& shop_place : shop_place_info.shop_places) {
    if (geom::point_inside_rect(player_info.position, shop_place.trigger)) {
      shop_place.state = OCCUPIED;
      shop_place.shop_id = shop::create_shop(item_info.models[item_info.items[player_info.item].model].shop_model);
      item::destroy_item(item_info.items[player_info.item].id);
      player_info.item = 0;
      return;
    }
  }
  drop_item();
}

void item_interaction() {
  if(item::exists_item(player_info.item)) {
    if(item_info.models[item_info.items[player_info.item].model].type == SHOP)
      use_item();
    else
      drop_item();
  }
  else {
    geom::Point position = player_info.position;

    u32 item = item::closest_item(position);
    if(!item)
      return;
    if(item::dist_to_item(position, item) < player_info.item_max_dist) {
      item::hold_item(item);
      player_info.item = item;
    }
  }
}

}
