#pragma once

#include <vector>

#include <SDL.h>

#include "shaders.h"
#include "types.h"

struct RenderContainer {
  // window info
  SDL_Window* window = nullptr;
  SDL_GLContext gl_context;

  // render info
  GLuint program_id = 0;

  std::vector<f32> vertex_buffer;
  std::vector<f32> color_buffer;
  std::vector<f32> uv_buffer;
  std::vector<GLuint> element_buffer;

  // textures
  std::vector<GLuint> texture;
  std::vector<u32>    texture_w;
  std::vector<u32>    texture_h;

  // draw call
  std::vector<u32> draw_texture;
  std::vector<u32> draw_start_element;
  std::vector<u32> draw_count_element;
};


void render_setup();
void render_cleanup();

u32 render_load_image(const char* filename);

void add_to_render(s32 x, s32 y, s32 w, s32 h, u32 texture);
void render();
