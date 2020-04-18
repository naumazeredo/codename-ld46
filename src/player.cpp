#include "player.h"

#include "input.h"
#include "render.h"

namespace player {

const int NUM_DIRECTIONS = 4;

enum Direction {UP, DOWN, LEFT, RIGHT};
Direction dir;

SDL_Scancode key[NUM_DIRECTIONS] = {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT};
u32 tex[NUM_DIRECTIONS];

int x, y, w = 50, h = 100;
int dx[NUM_DIRECTIONS] = {0, 0, -1, 1},
    dy[NUM_DIRECTIONS] = {1, -1, 0, 0};
int speed = 2;

void setup() {
  tex[0] = render::load_image("assets/gfx/template-32x32-up.png");
  tex[1] = render::load_image("assets/gfx/template-32x32-down.png");
  tex[2] = render::load_image("assets/gfx/template-32x32-left.png");
  tex[3] = render::load_image("assets/gfx/template-32x32-right.png");

  render::add_to_render(x, y, w, h, tex[RIGHT]);
}

void update() {
  for(int i = 0; i < NUM_DIRECTIONS; i++) {
    if(input::is_key_pressed(key[i])) {
      x += dx[i]*speed, y += dy[i]*speed;
      dir = (Direction)i;
    }
  }

  render::add_to_render(x, y, w, h, tex[dir]);
}

}
