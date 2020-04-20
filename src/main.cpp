//Using SDL and standard IO
#include <cstdio>

#include <SDL.h>

#include "externs.h"

int bg_tex;

void setup();
void load_audio();
void run();

int main(int argc, char* args[]) {
  setup();

  run();

  render::cleanup();

  return 0;
}

void setup() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  input::setup();
  render::setup();
  audio::setup();
  shop::setup();
  shop_place::setup();
  physics::setup();
  game::setup();

  texture::load();

  bound::setup();
  item::setup();
  player::setup();
  king::setup();
  enemy::setup();

  load_audio();

  bg_tex = render::load_image("assets/gfx/bg.png");
}

void load_audio() {
  audio::load_music("assets/sfx/MetalTheme.ogg");
  audio::load_sfx("assets/sfx/player-shoot.wav");
  audio::load_sfx("assets/sfx/piercing-shoot.wav");
}

void test_setup() {
  item::create_item(7, { 200.0, 100.0 });
  item::create_item(2, { 220.0, 100.0 });
  item::create_item(3, { 200.0, 120.0 });
  /*
  item::create_item(0, { 200.0, 100.0 });
  item::create_item(1, { 300.0, 100.0 });
  item::create_item(2, { 400.0, 100.0 });
  item::create_item(3, { 500.0, 100.0 });
  item::create_item(4, { 600.0, 100.0 });
  item::create_item(5, { 700.0, 100.0 });
  */

}

void debug_window() {
  debug_info.show_window = true;

  debug::add_window(game_time::debug_window);
  debug::add_window(game::debug_window);
  debug::add_window(input::debug_window);
  debug::add_window(render::debug_window);
  debug::add_window(audio::debug_window);
  debug::add_window(item::debug_window);
  debug::add_window(enemy::debug_window);
  debug::add_window(player::debug_window);
  debug::add_window(king::debug_window);
  debug::add_window(shop::debug_window);
  debug::add_window(shop_place::debug_window);
}

void run() {
  test_setup();
  debug_window();

  u32 p = 0;
  u8 cnt = 1;

  game_time::new_frame();

  u8 running = 1;
  game_time::new_frame();
  while (running) {
    game_time::new_frame();

    // Input
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      debug::handle_input(&event);

      if (event.type == SDL_QUIT) {
        running = 0;
      }

      if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
          case SDLK_ESCAPE:
            running = 0;
          break;
        }
      }
    }

    input::handle_input();

    player::update();
    item::update();
    enemy::update();
    king::update();
    game::update();
    animation::update();
    shop_place::update();
    physics::update();
    hud::update();

    enemy::render();
    player::render();
    item::render();
    game::render();
    shop_place::render();
    animation::render();

    render::add_to_render(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, bg_tex, 9999999);

    // Rendering
    render::render();
  }
}
