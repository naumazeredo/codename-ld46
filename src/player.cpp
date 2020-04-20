#include "player.h"

#include "externs.h"

PlayerInfo player_info;

namespace player {

void debug_window() {
  if (ImGui::TreeNode("Player")) {
    ImGui::Point("position", &player_info.position);
    ImGui::DragU32("w", &player_info.w);
    ImGui::DragU32("h", &player_info.h);

    //ImGui:: enum
    ImGui::InputInt("speed", &player_info.speed);
    ImGui::Text("is holdind item: %d", (int)player_info.is_holding_item);

    if (player_info.is_holding_item)
      ImGui::Text("holding item: %u", player_info.holding_item_id);

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

  player_info.is_holding_item = false;

  player_info.textures[0] = TEX_ARROW_UP;
  player_info.textures[1] = TEX_ARROW_DOWN;
  player_info.textures[2] = TEX_ARROW_LEFT;
  player_info.textures[3] = TEX_ARROW_RIGHT;
}

void render() {
  render::add_to_render(player_info.position.x - player_info.w / 2, player_info.position.y, player_info.w, player_info.h, player_info.textures[player_info.direction]);
}

void update() {
  if (player_info.is_holding_item) {
    item::update_position(player_info.holding_item_id, player_info.position + player_info.item_position);
  }
}

void drop_item() {
  if(!item::drop_item(player_info.holding_item_id)) return;

  item::update_position(player_info.holding_item_id, player_info.position);

  player_info.is_holding_item = false;
}

void use_item() {
  for(auto& shop_place : shop_place_info.shop_places) {
    if (geom::point_inside_rect(player_info.position, shop_place.trigger)) {
      shop_place.state = OCCUPIED;
      shop_place.shop_id = shop::create_shop(item_info.models[item_info.items[player_info.holding_item_id].model].shop_model);

      item::destroy_item(item_info.items[player_info.holding_item_id].id);
      player_info.is_holding_item = false;
      return;
    }
  }
  drop_item();
}

void item_interaction() {
  if(player_info.is_holding_item) {
    if(!item::item_exists(player_info.holding_item_id)) {
      printf("player holding item doesn't exists!\n");
      player_info.is_holding_item = false;
    }

    auto [found_model, model] = item::get_model_by_item_id(player_info.holding_item_id);
    if(found_model and model.type == SHOP)
      use_item();
    else
      drop_item();
  }
  else {
    geom::Point position = player_info.position;

    u32 item_id = item::closest_item(position); // @TODO(naum): refactor like enemy: closest_in_range
    if(!item_id)
      return;
    if(item::dist_to_item(position, item_id) < PLAYER_HOLD_MAX_DIST) {
      item::hold_item(item_id);
      player_info.is_holding_item = true;
      player_info.holding_item_id = item_id;
    }
  }
}

}
