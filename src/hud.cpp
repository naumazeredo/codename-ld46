#include "hud.h"

#include "externs.h"

namespace hud {

void setup() {}

int DIGIT_WIDTH = 7;
int DIGIT_HEIGHT = 15;

void display_digit(int x, int y, int digit) {
    switch (digit) {
        case 0:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_0, -3e5);
            break;
        case 1:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_1, -3e5);
            break;
        case 2:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_2, -3e5);
            break;
        case 3:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_3, -3e5);
            break;
        case 4:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_4, -3e5);
            break;
        case 5:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_5, -3e5);
            break;
        case 6:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_6, -3e5);
            break;
        case 7:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_7, -3e5);
            break;
        case 8:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_8, -3e5);
            break;
        case 9:
            render::add_to_render(x, y, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_9, -3e5);
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
  DIGIT_WIDTH = 7;
  DIGIT_HEIGHT = 15;
  int health_width = 480, health_height = 48;
  int health_x = SCREEN_WIDTH / 2 - health_width / 2;
  int health_y = SCREEN_HEIGHT - health_height;

  int coins_width = 160, coins_height = 64;
  int coins_x = SCREEN_WIDTH - coins_width;
  int coins_y = SCREEN_HEIGHT - coins_height;

  render::add_to_render(health_x, health_y, health_width, health_height, TextureCode::TEX_HUD_HEALTH, -3e5);
    render::add_to_render(coins_x, coins_y, coins_width, coins_height, TextureCode::TEX_HUD_COINS, -2e5);
  render::add_to_render(SCREEN_WIDTH - 40, SCREEN_HEIGHT - 60, 40, 40, TextureCode::TEX_MONEY, -3e5);
  if (player_info.money) {
    display_number(SCREEN_WIDTH - 45, SCREEN_HEIGHT - 28, player_info.money);
  } else {
    render::add_to_render(SCREEN_WIDTH - 45, SCREEN_HEIGHT - 28, DIGIT_WIDTH, DIGIT_HEIGHT, TextureCode::TEX_DIGIT_0, -3e5);
  }

  DIGIT_WIDTH = 10;
  DIGIT_HEIGHT = 23;

  render::add_to_render(2, SCREEN_HEIGHT - 25, 200, 23, TextureCode::TEX_TIME_UNTIL);
  int to_display = 0;
  if(u32(game_info.time_until_next_wave) == u32(10) or u32(game_info.time_until_next_wave) < u32(0))
  	to_display = 0;
  else
  	to_display = u32(game_info.time_until_next_wave);
  display_number(210, SCREEN_HEIGHT - 25, std:: max(0, to_display));
}

} // namespace hud