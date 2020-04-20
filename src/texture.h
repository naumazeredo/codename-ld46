#pragma once

enum TextureCode {
  TEX_BLANK,
  TEX_ARROW_UP,
  TEX_ARROW_DOWN,
  TEX_ARROW_LEFT,
  TEX_ARROW_RIGHT,
  TEX_TRAP,
  TEX_TURRET,
  TEX_SHOP,
  TEX_FOOD,
  // Add a new texture above this line
  TEX_NUM_TEXTURES
};

namespace texture {

void load();

}
