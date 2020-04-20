#pragma once

#include "types.h"
#include "geom.h"

const f32 PLAYER_HOLD_MAX_DIST = 40;

struct PlayerInfo {
  Direction direction;
  u32 textures[NUM_DIRECTIONS];

  bool is_holding_item;
  u32 holding_item_id;

  geom::Point position;
  geom::Point item_position;

  u32 w, h;
  int speed;
};

namespace player {

void setup();
void render();
void update();
void debug_window();

void item_interaction(); // @TODO(naum): remove this. Do into some interaction system

}
