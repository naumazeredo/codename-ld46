#include "player.h"

#include "externs.h"
#include "types.h"

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
    ImGui::Text("Item: %u", player_info.holding_item_id);

    ImGui::Text("Dir x: %d", player_info.dir_x);
    ImGui::Text("Dir y: %d", player_info.dir_y);

    ImGui::TreePop();
  }
}

void setup() {
  player_info.w = 56;
  player_info.h = 56;
  player_info.position = {(float) SCREEN_WIDTH/3 - player_info.w/2,
                          (float) SCREEN_HEIGHT/3 - player_info.h/2};
  player_info.direction = Direction::DOWN;
  player_info.speed = 100;

  player_info.money = 100;

  player_info.item_position = {0, (float) player_info.h};

  player_info.is_holding_item = false;
  auto idle_animation = animation::generate_animation_from_files(
    "assets/gfx/animations/rippedcat_idle",
    4
  );

  auto walking_animation = animation::generate_animation_from_files(
    "assets/gfx/animations/rippedcat_walking",
    2
  );
  auto rect = geom::Rect{0, 0, (f32) player_info.w, (f32) player_info.h};

  std::vector<animation::Animation> animations{idle_animation, walking_animation};
  animation::AnimationSet set{rect, animations, 0};

  player_info.animation_set_id = add_animation_set(set);
}

void render() {
  //render::add_to_render(player_info.position.x - player_info.w / 2, player_info.position.y, player_info.w, player_info.h, player_info.textures[player_info.direction]);
}

void update() {
  if (player_info.is_holding_item) {
    item::update_position(player_info.holding_item_id, player_info.position + player_info.item_position);
  }

  if (player_info.dir_x || player_info.dir_y){
    animation::force_play(player_info.animation_set_id, 1);
  } else {
    animation::force_play(player_info.animation_set_id, 0);
  }

  bool flip_horizontal = player_info.dir_x == -1;

  animation::set_animation_pos(
      player_info.animation_set_id,
      player_info.position.x,
      player_info.position.y,
      (f32) player_info.w,
      (f32) player_info.h,
      flip_horizontal
  );
}

void drop_item() {
  if(!item::drop_item(player_info.holding_item_id)) return;

  item::update_position(player_info.holding_item_id, player_info.position);

  player_info.is_holding_item = false;
}

void use_item() {
  for(auto& shop_place : shop_place_info.shop_places) {
    if (geom::point_inside_convex_polygon(player_info.position, shop_place.trigger)) {
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
    if (geom::point_inside_convex_polygon(player_info.position, shop_place.trigger)) {
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
