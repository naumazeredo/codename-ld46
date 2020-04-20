#pragma once

#include "texture.h"
#include "types.h"
#include "physics.h"
#include "geom.h"
#include "game.h"

#include "time.h"
#include "render.h"
#include "audio.h"
#include "input.h"
#include "player.h"
#include "debug.h"
#include "shop.h"
#include "item.h"
#include "enemy.h"
#include "geom.h"
#include "game.h"
#include "shop_place.h"
#include "king.h"
#include "animation.h"
#include "bound.h"
#undef min
#undef max

extern TimeInfo time_info;
extern RenderInfo render_info;
extern AudioInfo audio_info;
extern InputInfo input_info;
extern PlayerInfo player_info;
extern DebugInfo debug_info;
extern ItemInfo item_info;
extern EnemyInfo enemy_info;
extern ShopInfo shop_info;
extern ShopPlaceInfo shop_place_info;
extern KingInfo king_info;
extern GameInfo game_info;
extern AnimationInfo animation_info;
extern BoundInfo bound_info;
