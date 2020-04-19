//Using SDL and standard IO
#include <cstdio>

#include <SDL.h>

#include "externs.h"

void setup();
void load_textures();
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
  physics::setup();
  game::setup();

  load_textures();
  load_audio();
}

void load_textures() {
  // @TODO(naum): store into names variables
}

void load_audio() {
  audio::load_music("assets/sfx/MetalTheme.ogg");
  audio::load_sfx("assets/sfx/player-shoot.wav");
  audio::load_sfx("assets/sfx/piercing-shoot.wav");
}

void test_setup() {
  item::create_item(1, { 700.0, 100.0 });
  item::create_item(2, { 700.0, 600.0 });
}

void run() {
  test_setup();

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

    shop::update();
    player::update();
    item::update();
    enemy::update();
    physics::update();
    game::update();

    enemy::render();
    player::render();
    item::render();
    game::render();

    // Rendering
    render::render();
  }
}
