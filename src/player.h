#pragma once

#include "texture.h"
#include "types.h"
#include "geom.h"

const f32 INTERACTION_DISTANCE = 40;

struct PlayerInfo {
  Direction direction;
  TextureCode textures[NUM_DIRECTIONS];

  bool is_holding_item;
  u32 holding_item_id;

  u32 animation_set_id;

  geom::Point position;
  geom::Point item_position;

  u32 w, h;
  int speed;
  int money;
  float item_max_dist;
  s8 dir_x, dir_y;
};

namespace player {

void setup();
void render();
void update();
void debug_window();

void world_interaction(); // @TODO(naum): remove this. Do into some interaction system

}
