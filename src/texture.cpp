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
  render::load_image("assets/gfx/temp-shop-top.png");
  render::load_image("assets/gfx/food-32x32-for-replace.png");
}

}
