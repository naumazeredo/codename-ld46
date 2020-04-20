#include "king.h"

#include "externs.h"

KingInfo king_info;

namespace king {

void debug_window() {
  if (ImGui::TreeNode("King")) {
    ImGui::InputFloat("x", &king_info.position.x, 1.0f, 10.0f);
    ImGui::InputFloat("y", &king_info.position.y, 1.0f, 10.0f);
    ImGui::InputInt("w", &king_info.w);
    ImGui::InputInt("h", &king_info.h);

    ImGui::TreePop();
  }
}
void setup() {
  king_info.w = 100;
  king_info.h = 150;
  king_info.position = {(float) SCREEN_WIDTH/2 - king_info.w/2,
    (float) SCREEN_HEIGHT/2 - king_info.h/2};
  king_info.texture = TEX_ARROW_DOWN;

  debug::add_window(debug_window);
}

void render() {
  render::add_to_render(king_info.position.x - king_info.w / 2, king_info.position.y, king_info.w, king_info.h, king_info.texture);
}

}
