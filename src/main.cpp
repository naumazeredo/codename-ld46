//Using SDL and standard IO
#include <stdio.h>

#include <SDL.h>

#include "debug.h"
#include "render.h"
#include "audio.h"
#include "input.h"
#include "player.h"

u32 tex;
int x = 0, y = 0;

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
  player::setup();

  load_textures();
  load_audio();
}

void load_textures() {
  // @TODO(naum): store into names variables
  render::load_image("assets/gfx/template-32x32-up.png");
  render::load_image("assets/gfx/template-32x32.png");

  tex = 0;
}

void load_audio() {
  audio::load_music("assets/sfx/MetalTheme.ogg");
  audio::load_sfx("assets/sfx/player-shoot.wav");
  audio::load_sfx("assets/sfx/piercing-shoot.wav");
}

void run() {
  u32 p = 0;
  u8 cnt = 1;

  u8 running = 1;
  while (running) {
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

    //add_to_render(x, y, 200, 200, tex);

    input::update();
    player::update();

    // Rendering
    render::render();
  }
}
