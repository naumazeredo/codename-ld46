#pragma once

enum class TextureCode {
  TEX_BLANK = 0,
  TEX_ARROW_UP,
  TEX_ARROW_DOWN,
  TEX_ARROW_LEFT,
  TEX_ARROW_RIGHT,
  TEX_TRAP,
  TEX_TURRET,
  TEX_TRAP_SHOP,
  TEX_CANDY,
  TEX_MONEY,
  TEX_FACTORY,
  TEX_LOG,
  TEX_LOG_SHADOW,
  TEX_KING_ROCK,
  TEX_KING_ROCK_SHADOW,
  TEX_CONSTRUCTION_SITE,
  TEX_TURRET_SHOP,
  // Add a new texture above this line
  TEX_NUM_TEXTURES,
  INVALID
};

namespace texture {

void load();

}
