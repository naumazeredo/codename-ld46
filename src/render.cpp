// @TODO(naum): multiple program_ids
// @TODO(naum): remove std::vector and use a better, more performant, data structure

#include "render.h"

#include <SDL.h>
#include <stb/stb_image.h>

#include "shaders.h"
#include "debug.h"

const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

RenderContainer render_container;

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


internal
void render_setup_window() {
  // GL context

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GL_SetSwapInterval(1); // vsync

  auto& window = render_container.window;
  window = SDL_CreateWindow("Codename Pets",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);

  if (window == nullptr) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  auto& gl_context = render_container.gl_context;
  gl_context = SDL_GL_CreateContext(window);

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);

  if (gl3wInit() != 0) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    exit(1);
  }
}

internal
void render_cleanup_window() {
  SDL_GL_DeleteContext(render_container.gl_context);
  SDL_DestroyWindow(render_container.window);
  SDL_Quit();
}

void render_setup() {
  // window
  render_setup_window();

  // debug
  setup_debug(render_container.window, render_container.gl_context);

  // render
  glGenVertexArrays(1, &vertex_array_object);

  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &color_buffer_object);
  glGenBuffers(1, &uv_buffer_object);
  glGenBuffers(1, &element_buffer_object);

  program_id = load_shaders("assets/shaders/regular.vs", "assets/shaders/regular.fs");

  texture_id    = glGetUniformLocation(program_id, "tex");
  position_attr = glGetAttribLocation(program_id, "position");
  color_attr    = glGetAttribLocation(program_id, "color");
  uv_attr       = glGetAttribLocation(program_id, "uv");

  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);
}

void render_cleanup() {
  // render
  glDeleteProgram(program_id);

  glDeleteVertexArrays(1, &vertex_array_object);

  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &color_buffer_object);
  glDeleteBuffers(1, &uv_buffer_object);
  glDeleteBuffers(1, &element_buffer_object);

  // debug
  cleanup_debug();

  // window
  render_cleanup_window();
}


u32 render_load_image(const char* filename) {
  int w, h;
  unsigned char* texture_data = stbi_load(filename, &w, &h, nullptr, 4);

  if (texture_data == nullptr) {
    printf("Could not load texture: %s\n", filename);
    return 0;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  stbi_image_free(texture_data);

  render_container.texture.push_back(texture);
  render_container.texture_w.push_back(w);
  render_container.texture_h.push_back(h);

  return render_container.texture.size() - 1;
}

void add_to_render(s32 x, s32 y, s32 w, s32 h, u32 texture) {
  auto start_vertex = render_container.vertex_buffer.size() / 3;

  f32 x0 = 2 * x/f32(SCREEN_WIDTH) - 1;
  f32 y0 = 2 * y/f32(SCREEN_HEIGHT) - 1;

  f32 x1 = 2 * (x+w)/f32(SCREEN_WIDTH) - 1;
  f32 y1 = 2 * (y+h)/f32(SCREEN_HEIGHT) - 1;

  // first triangle
  auto& vertex_buffer = render_container.vertex_buffer;
  vertex_buffer.push_back(x0);
  vertex_buffer.push_back(y0);
  vertex_buffer.push_back(0);

  vertex_buffer.push_back(x1);
  vertex_buffer.push_back(y0);
  vertex_buffer.push_back(0);

  vertex_buffer.push_back(x1);
  vertex_buffer.push_back(y1);
  vertex_buffer.push_back(0);

  vertex_buffer.push_back(x0);
  vertex_buffer.push_back(y1);
  vertex_buffer.push_back(0);


  auto& uv_buffer = render_container.uv_buffer;
  uv_buffer.push_back(0);
  uv_buffer.push_back(1);

  uv_buffer.push_back(1);
  uv_buffer.push_back(1);

  uv_buffer.push_back(1);
  uv_buffer.push_back(0);

  uv_buffer.push_back(0);
  uv_buffer.push_back(0);


  auto& color_buffer = render_container.color_buffer;
  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);

  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);

  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);

  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);
  color_buffer.push_back(1);


  auto& element_buffer = render_container.element_buffer;
  element_buffer.push_back(start_vertex+0);
  element_buffer.push_back(start_vertex+1);
  element_buffer.push_back(start_vertex+2);

  element_buffer.push_back(start_vertex+2);
  element_buffer.push_back(start_vertex+3);
  element_buffer.push_back(start_vertex+0);

  assert(texture < render_container.texture.size());
  render_container.draw_texture.push_back(texture);

  render_container.draw_count_element.push_back(6);

  if (render_container.draw_start_element.empty()) {
    render_container.draw_start_element.push_back(0);
  } else {
    render_container.draw_start_element.push_back(render_container.draw_start_element.back() +
                                                  render_container.draw_count_element.back());
  }
}

internal
void render_start_new_frame() {
  render_container.vertex_buffer.clear();
  render_container.color_buffer.clear();
  render_container.uv_buffer.clear();
  render_container.element_buffer.clear();

  render_container.draw_texture.clear();
  render_container.draw_start_element.clear();
  render_container.draw_count_element.clear();
}

internal
void render_bind_buffers() {
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * render_container.vertex_buffer.size(),
               &render_container.vertex_buffer[0],
               GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * render_container.uv_buffer.size(),
               &render_container.uv_buffer[0],
               GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * render_container.color_buffer.size(),
               &render_container.color_buffer[0],
               GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(GLuint) * render_container.element_buffer.size(),
               &render_container.element_buffer[0],
               GL_STREAM_DRAW);
}

void render() {
  glClearColor(1, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  render_bind_buffers();

  glBindVertexArray(vertex_array_object);

  glUseProgram(program_id);

  // vertex position
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // uv
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_object);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // colors
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);

  // texture
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(texture_id, 0);

  for (u32 i = 0; i < render_container.draw_texture.size(); i++) {
    auto tex = render_container.texture[render_container.draw_texture[i]];
    auto start = render_container.draw_start_element[i];
    auto count = render_container.draw_count_element[i];

    // texture
    glBindTexture(GL_TEXTURE_2D, tex);

    // draw call
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(intptr_t)(start * sizeof(GLuint)));
    //glDrawElementsBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, start ? 4 : 0);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  // 
  render_debug_window(render_container.window);

  SDL_GL_SwapWindow(render_container.window);

  render_start_new_frame();
}

/*
// Maintain to remember how to do
internal
void update_buffers() {
  std::vector<f32> new_vertex_buffer = vertex_buffer;
  for (u32 i = 0; i < new_vertex_buffer.size(); i += 3) {
    new_vertex_buffer[i  ] += offset_x;
    new_vertex_buffer[i+1] += offset_y;
  }

  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(f32) * new_vertex_buffer.size(), &new_vertex_buffer[0]);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
*/

