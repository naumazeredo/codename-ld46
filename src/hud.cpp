#include "hud.h"

#include "externs.h"

namespace hud {

void setup() {}

const int DIGIT_WIDTH = 10;
const int DIGIT_HEIGHT = 23;

void display_digit(int x, int y, int digit) {
    switch (digit) {
        case 0:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_0, 9999);
            break;
        case 1:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_1, 9999);
            break;
        case 2:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_2, 9999);
            break;
        case 3:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_3, 9999);
            break;
        case 4:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_4, 9999);
            break;
        case 5:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_5, 9999);
            break;
        case 6:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_6, 9999);
            break;
        case 7:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_7, 9999);
            break;
        case 8:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_8, 9999);
            break;
        case 9:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_9, 9999);
            break;
        default:
            break;
    }
}

void display_number(int x, int y, int number) {
  if (!number) {
    return;
  }

  int digit = number % 10;
  display_digit(x, y, digit);
  display_number(x - DIGIT_WIDTH, y, number / 10);
}

void update() {
  int width = 800, height = 100;
  int x = SCREEN_WIDTH / 2 - width / 2;
  int y = SCREEN_HEIGHT - height + 20;

  render::add_to_render(x, y, width, height, TextureCode::TEX_HUD, 9999);
  display_number(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 50, 253);
}

} // namespace hud