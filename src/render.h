#pragma once

#include <vector>
#include <utility>

#include <SDL.h>
#include <GL/gl3w.h>

#include "texture.h"
#include "types.h"

// @TODO(naum): change this to be a variable and changed during execution
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

struct Color {
  f32 r,g,b,a;
};

const Color WHITE = Color{1,1,1,1};

struct RenderInfo {
  // window info
  SDL_Window* window;
  SDL_GLContext gl_context;

  // textures
  std::vector<GLuint> texture;
  std::vector<u32>    texture_w;
  std::vector<u32>    texture_h;

  // render info
  std::vector<f32> vertex_buffer;
  std::vector<f32> color_buffer;
  std::vector<f32> uv_buffer;
  std::vector<GLuint> element_buffer;

  std::vector<s32> order;

  // draw call
  std::vector<u32> draw_texture;
  std::vector<u32> draw_start_element;
  std::vector<u32> draw_count_element;

  // GL objects
  GLuint vertex_array_object;

  GLuint vertex_buffer_object;
  GLuint color_buffer_object;
  GLuint uv_buffer_object;
  GLuint element_buffer_object;

  GLuint program_id;
  GLuint texture_id;

  GLint position_attr;
  GLint color_attr;
  GLint uv_attr;
};

namespace render {

void setup();
void cleanup();
void debug_window();
void render();

u32 load_image(const char* filename);

void add_to_render(s32 x, s32 y, s32 w, s32 h, u32 texture, s32 z = -9900, Color tint = WHITE, bool flip_horizontal = false);
void add_to_render(s32 x, s32 y, s32 w, s32 h, TextureCode tex, s32 z = -9800, Color tint = WHITE);
void render();

}
