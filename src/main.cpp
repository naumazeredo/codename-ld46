//Using SDL and standard IO
#include <cstdio>

#include <SDL.h>

#include "externs.h"
#include "king.h"

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
  item::setup();
  enemy::setup();
  player::setup();
  shop::setup();
  shop_place::setup();
  physics::setup();
  game::setup();
  king::setup();

  texture::load();

  load_audio();

}

void load_audio() {
  audio::load_music("assets/sfx/MetalTheme.ogg");
  audio::load_sfx("assets/sfx/player-shoot.wav");
  audio::load_sfx("assets/sfx/piercing-shoot.wav");
}

void test_setup() {
  item::create_item(2, { 700.0, 100.0 });
  item::create_item(3, { 700.0, 600.0 });
  item::create_item(4, { 800.0, 100.0 });

  animation::debug_animation();

}

void debug_window() {
  debug_info.show_window = true;

  debug::add_window(input::debug_window);
  debug::add_window(render::debug_window);
  debug::add_window(audio::debug_window);
  debug::add_window(item::debug_window);
  debug::add_window(enemy::debug_window);
  debug::add_window(player::debug_window);
  debug::add_window(shop_place::debug_window);
  debug::add_window(game::debug_window);
  debug::add_window(king::debug_window);
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
    physics::update();
    game::update();
    animation::update();

    enemy::render();
    player::render();
    item::render();
    game::render();
    shop_place::render();
    king::render();
    animation::render();

    // Rendering
    render::render();
  }
}
