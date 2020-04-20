#include "player.h"

#include "externs.h"

PlayerInfo player_info;

namespace player {

void debug_window() {
  if (ImGui::TreeNode("Player")) {
    ImGui::Point("position", &player_info.position);
    ImGui::DragU32("w", &player_info.w);
    ImGui::DragU32("h", &player_info.h);

    ImGui::InputInt("speed", &player_info.speed);
    ImGui::Text("is holdind item: %d", (int)player_info.is_holding_item);

    if (player_info.is_holding_item)
      ImGui::Text("holding item: %u", player_info.holding_item_id);

    ImGui::InputInt("money", &player_info.money);

    ImGui::TreePop();
  }
}

void setup() {
  player_info.w = 50;
  player_info.h = 100;
  player_info.position = {(float) SCREEN_WIDTH/3 - player_info.w/2,
                          (float) SCREEN_HEIGHT/3 - player_info.h/2};
  player_info.direction = Direction::DOWN;
  player_info.speed = 100;

  player_info.money = 100;

  player_info.item_position = {0, (float) player_info.h};

  player_info.is_holding_item = false;

  player_info.textures[0] = TextureCode::TEX_ARROW_UP;
  player_info.textures[1] = TextureCode::TEX_ARROW_DOWN;
  player_info.textures[2] = TextureCode::TEX_ARROW_LEFT;
  player_info.textures[3] = TextureCode::TEX_ARROW_RIGHT;
}

void render() {
  render::add_to_render(player_info.position.x - player_info.w / 2, player_info.position.y, player_info.w, player_info.h, player_info.textures[(int)player_info.direction]);
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
      auto [exist, item_model] = item::get_model_by_item_id(player_info.holding_item_id);

      if (exist) {
        shop_place.state = ShopPlaceState::OCCUPIED;
        shop_place.shop_id = shop::create_shop(item_model.shop_model_id);

        item::destroy_item(item_info.items[player_info.holding_item_id].id);
        player_info.is_holding_item = false;
      }

      return;
    }
  }
  drop_item();
}

bool try_buy_item(u32 shop_id) {
  auto [found_shop_model, model] = shop::get_model_by_shop_id(shop_id);
  if (!found_shop_model) {
    return false;
  }
  if(model.type != ShopType::SHOP) {
    return false;
  }
  if(player_info.money < model.sell_price or player_info.holding_item_id) {
    return false;
  }

  player_info.money -= model.sell_price;
  u32 item_id = item::create_item(model.item_model_id, player_info.position);
  item::hold_item(item_id);
  return true;
}

void shop_interaction() {
  for(auto& shop_place : shop_place_info.shop_places) {
    if (geom::point_inside_rect(player_info.position, shop_place.trigger)) {
      if (shop_place.state == ShopPlaceState::OCCUPIED) {
        try_buy_item(shop_place.shop_id);
      }
    }
  }
}

void item_interaction() {
  if(player_info.is_holding_item) {
    if(!item::item_exists(player_info.holding_item_id)) {
      printf("player holding item doesn't exists!\n");
      player_info.is_holding_item = false;
    }

    auto [found_model, model] = item::get_model_by_item_id(player_info.holding_item_id);
    if (found_model and model.type == ItemType::SHOP)
      use_item();
    else
      drop_item();
  }
  else {
    shop_interaction();

    geom::Point position = player_info.position;
    u32 item_id = item::closest_item(position); // @TODO(naum): refactor like enemy: closest_in_range
    if(!item_id)
      return;

    if(item::dist_to_item(position, item_id) < PLAYER_HOLD_MAX_DIST) {
      auto [_, item_model] = item::get_model_by_item_id(item_id);

      if(item_model.type == ItemType::MONEY) {
        player_info.money += MONEY_PER_COIN;
        item::destroy_item(item_id);
        return;
      }

      item::hold_item(item_id);
      player_info.is_holding_item = true;
      player_info.holding_item_id = item_id;
    }
  }
}

}
