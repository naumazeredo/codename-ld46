#include "player.h"

#include "input.h"
#include "render.h"

namespace player {

const int N_DIRECTIONS = 4;
enum direction {UP, DOWN, LEFT, RIGHT};
direction dir;

SDL_Scancode key[N_DIRECTIONS] = {SDL_SCANCODE_UP, SDL_SCANCODE_DOWN, SDL_SCANCODE_LEFT, SDL_SCANCODE_RIGHT};
static u32 tex[N_DIRECTIONS];
int speed = 2;
int dx[N_DIRECTIONS] = {0, 0, -1, 1}, dy[N_DIRECTIONS] = {1, -1, 0, 0};
static int x, y, w = 50, h = 100;

void setup() {
  tex[0] = render_load_image("assets/gfx/template-32x32-up.png");
  tex[1] = render_load_image("assets/gfx/template-32x32-down.png");
  tex[2] = render_load_image("assets/gfx/template-32x32-left.png");
  tex[3] = render_load_image("assets/gfx/template-32x32-right.png");

  add_to_render(x, y, w, h, tex[RIGHT]);
}
void update() {
  for(int i = 0; i < N_DIRECTIONS; i++) {
    if(is_pressed(key[i])) {
      x += dx[i]*speed, y += dy[i]*speed;
      dir = (direction)i;
    }
  }
  add_to_render(x, y, w, h, tex[dir]);
}
}
