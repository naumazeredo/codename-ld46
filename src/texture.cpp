#include "texture.h"

#include "render.h"


namespace texture {

void load() {
  render::load_image("assets/gfx/blank.png");
  render::load_image("assets/gfx/template-32x32-up.png");
  render::load_image("assets/gfx/template-32x32-down.png");
  render::load_image("assets/gfx/template-32x32-left.png");
  render::load_image("assets/gfx/template-32x32-right.png");
  render::load_image("assets/gfx/temp-trap.png");
  render::load_image("assets/gfx/turret-must-replace.png");
  render::load_image("assets/gfx/store_trap.png");
  render::load_image("assets/gfx/temp-candy.png");
  render::load_image("assets/gfx/temp-coin.png");
  render::load_image("assets/gfx/candy_factory.png");
  render::load_image("assets/gfx/log.png");
  render::load_image("assets/gfx/log-shadow.png");
  render::load_image("assets/gfx/king-rock.png");
  render::load_image("assets/gfx/king-rock-shadow.png");
  render::load_image("assets/gfx/construction_site_1.png");
  render::load_image("assets/gfx/store_turret.png");
  render::load_image("assets/gfx/hud_health.png");
  render::load_image("assets/gfx/hud_coins.png");
  render::load_image("assets/gfx/digit_0.png");
  render::load_image("assets/gfx/digit_1.png");
  render::load_image("assets/gfx/digit_2.png");
  render::load_image("assets/gfx/digit_3.png");
  render::load_image("assets/gfx/digit_4.png");
  render::load_image("assets/gfx/digit_5.png");
  render::load_image("assets/gfx/digit_6.png");
  render::load_image("assets/gfx/digit_7.png");
  render::load_image("assets/gfx/digit_8.png");
  render::load_image("assets/gfx/digit_9.png");
  render::load_image("assets/gfx/game_over_screen.png");
}

}
