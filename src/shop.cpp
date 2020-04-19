#include <imgui.h>
#include <vector>

#include "shop.h"
#include "render.h"
#include "debug.h"


Shop::Shop(Point p, Rect c, Rect t): center(p), collider(c), trigger(t) {

  textures[0] = render::load_image("assets/gfx/template-32x32-up.png");
  textures[1] = render::load_image("assets/gfx/template-32x32-down.png");

  state = shop::CLOSED;
}

void Shop::sell() {}

ShopsInfo shops_info;

namespace shop {

void debug_window() {
  ImGui::Text("Shops");

  int cnt = 0;
  for(auto& shop : shops_info.shops) {
    ImGui::Text("shop %d: ", ++cnt);
    ImGui::Text("x %.2lf", shop.center.x);
    ImGui::Text("y %.2lf", shop.center.y);
    ImGui::Text("w %.2lf", shop.collider.width);
    ImGui::Text("h %.2lf", shop.collider.height);
    ImGui::Text("texture %s", shop.state ? "OPEN" : "CLOSED");
  }

  //ImGui:: enum
}

void setup() {
  float width = 300, height = 200;
  Point center = {(float) SCREEN_WIDTH/4,
                  (float) SCREEN_HEIGHT/2};
  Rect collider = {center, width, height};
  Rect trigger = {center, 500, 400};
  Shop first_shop = Shop(center, collider, trigger);
  shops_info.shops.push_back(first_shop);

  debug::add_window(debug_window);
}

void update() {
    for(auto& shop : shops_info.shops) {
        render::add_to_render(shop.center.x - shop.collider.width / 2, 
                              shop.center.y - shop.collider.height / 2, 
                              shop.collider.width, 
                              shop.collider.height, 
                              shop.textures[shop.state]);
    }
}

} // namespace shop