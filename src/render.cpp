// @TODO(naum): multiple program_ids
// @TODO(naum): remove std::vector and use a better, more performant, data structure
// @FIXME(naum): don't use iostream, stringstream or any of this crap

#include "render.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include <imgui.h>
#include <stb/stb_image.h>

#include "externs.h"

RenderInfo render_info;

namespace render {

void debug_window() {
  if (ImGui::TreeNode("Render")) {
    ImGui::Text("textures:");

    for (u32 i = 0; i < render_info.texture.size(); i++) {
      auto texture = render_info.texture[i];
      auto texture_w = render_info.texture_w[i];
      auto texture_h = render_info.texture_h[i];

      ImGui::Image((void*)(intptr_t)texture, ImVec2(32, 32 * texture_h / texture_w) );
    }

    ImGui::TreePop();
  }
}

GLuint load_shader(const char* shader_code, GLenum shader_type) {
  GLuint shader_id = glCreateShader(shader_type);

  glShaderSource(shader_id, 1, &shader_code, NULL);
  glCompileShader(shader_id);

  GLint result = GL_FALSE;
  int info_log_length;

  // Check Vertex Shader
  glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &info_log_length);

  if (info_log_length > 0) {
    std::vector<char> shader_error_message(info_log_length+1);
    glGetShaderInfoLog(shader_id, info_log_length, NULL, &shader_error_message[0]);

    // @FixIt should log
    printf("%s\n", &shader_error_message[0]);
  }

  return shader_id;
}

GLuint load_shaders(const char* vertex_file_path, const char* fragment_file_path){
  printf("Loading shaders...\n");

  // Read the Vertex Shader code from the file
  std::string vertex_shader_code;
  std::ifstream vertex_shader_stream(vertex_file_path, std::ios::in);
  if (vertex_shader_stream.is_open()) {
    std::stringstream sstr;
    sstr << vertex_shader_stream.rdbuf();
    vertex_shader_code = sstr.str();
    vertex_shader_stream.close();
  } else {
    printf("Could not open file: \"%s\".\n", vertex_file_path);
    return 0;
  }

  // Compile vertex shader
  printf("Compiling shader : %s\n", vertex_file_path);
  GLuint vertex_shader_id = load_shader(vertex_shader_code.c_str(), GL_VERTEX_SHADER);

  // Read the Fragment Shader code from the file
  std::string fragment_shader_code;
  std::ifstream fragment_shader_stream(fragment_file_path, std::ios::in);
  if (fragment_shader_stream.is_open()) {
    std::stringstream sstr;
    sstr << fragment_shader_stream.rdbuf();
    fragment_shader_code = sstr.str();
    fragment_shader_stream.close();
  }

  // Compile fragment shader
  printf("Compiling shader : %s\n", fragment_file_path);
  GLuint fragment_shader_id = load_shader(fragment_shader_code.c_str(), GL_FRAGMENT_SHADER);

  // Link the program
  printf("Linking program\n");
  GLuint program_id = glCreateProgram();
  glAttachShader(program_id, vertex_shader_id);
  glAttachShader(program_id, fragment_shader_id);
  glLinkProgram(program_id);

  // Check the program
  GLint result = GL_FALSE;
  int info_log_length;

  glGetProgramiv(program_id, GL_LINK_STATUS, &result);
  glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length > 0) {
    std::vector<char> program_error_message(info_log_length+1);
    glGetProgramInfoLog(program_id, info_log_length, NULL, &program_error_message[0]);

    // @FixIt logger
    printf("%s\n", &program_error_message[0]);
  }

  glDetachShader(program_id, vertex_shader_id);
  glDetachShader(program_id, fragment_shader_id);

  glDeleteShader(vertex_shader_id);
  glDeleteShader(fragment_shader_id);

  return program_id;
}

//internal
void setup_window() {
  // GL context

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

  SDL_GL_SetSwapInterval(1); // vsync

  auto& window = render_info.window;
  window = SDL_CreateWindow("Codename Pets",
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_OPENGL);

  if (window == nullptr) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    exit(1);
  }

  auto& gl_context = render_info.gl_context;
  gl_context = SDL_GL_CreateContext(window);

  SDL_GL_MakeCurrent(window, gl_context);
  SDL_GL_SetSwapInterval(1);

  if (gl3wInit() != 0) {
    fprintf(stderr, "Failed to initialize OpenGL loader!\n");
    exit(1);
  }
}

//internal
void cleanup_window() {
  SDL_GL_DeleteContext(render_info.gl_context);
  SDL_DestroyWindow(render_info.window);
  SDL_Quit();
}

void setup() {
  // window
  setup_window();

  // debug
  debug::setup(render_info.window, render_info.gl_context);

  // render
  glGenVertexArrays(1, &render_info.vertex_array_object);

  glGenBuffers(1, &render_info.vertex_buffer_object);
  glGenBuffers(1, &render_info.color_buffer_object);
  glGenBuffers(1, &render_info.uv_buffer_object);
  glGenBuffers(1, &render_info.element_buffer_object);

  auto& program_id = render_info.program_id;
  program_id = load_shaders("assets/shaders/regular.vs", "assets/shaders/regular.fs");

  render_info.texture_id    = glGetUniformLocation(program_id, "tex");
  render_info.position_attr = glGetAttribLocation(program_id, "position");
  render_info.color_attr    = glGetAttribLocation(program_id, "color");
  render_info.uv_attr       = glGetAttribLocation(program_id, "uv");

  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);
}

void cleanup() {
  // render
  glDeleteProgram(render_info.program_id);

  glDeleteVertexArrays(1, &render_info.vertex_array_object);

  glDeleteBuffers(1, &render_info.vertex_buffer_object);
  glDeleteBuffers(1, &render_info.color_buffer_object);
  glDeleteBuffers(1, &render_info.uv_buffer_object);
  glDeleteBuffers(1, &render_info.element_buffer_object);

  // debug
  debug::cleanup();

  // window
  cleanup_window();
}


u32 load_image(const char* filename) {
  int w, h;
  unsigned char* texture_data = stbi_load(filename, &w, &h, nullptr, 4);

  if (texture_data == nullptr) {
    printf("Could not load texture: %s\n", filename);
    return 0;
  }

  GLuint t;
  glGenTextures(1, &t);
  glBindTexture(GL_TEXTURE_2D, t);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  stbi_image_free(texture_data);

  render_info.texture.push_back(t);
  render_info.texture_w.push_back(w);
  render_info.texture_h.push_back(h);

  return render_info.texture.size() - 1;
}

void add_to_render(s32 x, s32 y, s32 w, s32 h, TextureCode tex, s32 z, Color tint) {
  add_to_render(x, y, w, h, (u32)tex, z, tint);
}

void add_to_render(s32 x, s32 y, s32 w, s32 h, u32 tex, s32 z, Color tint, bool flip_horizontal) {
  auto start_vertex = render_info.vertex_buffer.size() / 3;

  f32 x0 = 2 * x/f32(SCREEN_WIDTH) - 1;
  f32 y0 = 2 * y/f32(SCREEN_HEIGHT) - 1;

  f32 x1 = 2 * (x+w)/f32(SCREEN_WIDTH) - 1;
  f32 y1 = 2 * (y+h)/f32(SCREEN_HEIGHT) - 1;

  // first triangle
  auto& vertex_buffer = render_info.vertex_buffer;
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


  auto& uv_buffer = render_info.uv_buffer;
  uv_buffer.push_back(flip_horizontal);
  uv_buffer.push_back(1);

  uv_buffer.push_back(!flip_horizontal);
  uv_buffer.push_back(1);

  uv_buffer.push_back(!flip_horizontal);
  uv_buffer.push_back(0);

  uv_buffer.push_back(flip_horizontal);
  uv_buffer.push_back(0);


  auto& color_buffer = render_info.color_buffer;
  color_buffer.push_back(tint.r);
  color_buffer.push_back(tint.g);
  color_buffer.push_back(tint.b);
  color_buffer.push_back(tint.a);

  color_buffer.push_back(tint.r);
  color_buffer.push_back(tint.g);
  color_buffer.push_back(tint.b);
  color_buffer.push_back(tint.a);

  color_buffer.push_back(tint.r);
  color_buffer.push_back(tint.g);
  color_buffer.push_back(tint.b);
  color_buffer.push_back(tint.a);

  color_buffer.push_back(tint.r);
  color_buffer.push_back(tint.g);
  color_buffer.push_back(tint.b);
  color_buffer.push_back(tint.a);

  auto& element_buffer = render_info.element_buffer;
  element_buffer.push_back(start_vertex+0);
  element_buffer.push_back(start_vertex+1);
  element_buffer.push_back(start_vertex+2);

  element_buffer.push_back(start_vertex+2);
  element_buffer.push_back(start_vertex+3);
  element_buffer.push_back(start_vertex+0);

  if(tex >= render_info.texture.size()) {
    printf("[error] invalid texture! %u\n", tex);
    return;
  }

  auto& draw_texture = render_info.draw_texture;
  auto& draw_start_element = render_info.draw_start_element;
  auto& draw_count_element = render_info.draw_count_element;

  draw_texture.push_back((u32)tex);
  draw_count_element.push_back(6);

  if (draw_start_element.empty()) {
    draw_start_element.push_back(0);
  } else {
    draw_start_element.push_back(draw_start_element.back() +
                                 draw_count_element.back());
  }

  render_info.order.push_back(z);
}

//internal
void start_new_frame() {
  render_info.vertex_buffer.clear();
  render_info.color_buffer.clear();
  render_info.uv_buffer.clear();
  render_info.element_buffer.clear();
  render_info.order.clear();

  render_info.draw_texture.clear();
  render_info.draw_start_element.clear();
  render_info.draw_count_element.clear();
}

//internal
void bind_buffers() {
  glBindBuffer(GL_ARRAY_BUFFER, render_info.vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * render_info.vertex_buffer.size(),
               &render_info.vertex_buffer[0],
               GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, render_info.uv_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * render_info.uv_buffer.size(),
               &render_info.uv_buffer[0],
               GL_STREAM_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, render_info.color_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * render_info.color_buffer.size(),
               &render_info.color_buffer[0],
               GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_info.element_buffer_object);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(GLuint) * render_info.element_buffer.size(),
               &render_info.element_buffer[0],
               GL_STREAM_DRAW);
}

void render() {
  std::vector<u32> new_order (render_info.order.size());
  for (u32 i = 0; i < render_info.order.size(); i++) new_order[i] = i;

  std::sort(new_order.begin(), new_order.end(), [](u32 i, u32 j) { return (render_info.order[i] > render_info.order[j]); });

  glClearColor(1, 0, 1, 1);
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  //glEnable(GL_DEPTH_TEST);
  //glDepthFunc(GL_LESS);

  bind_buffers();

  glBindVertexArray(render_info.vertex_array_object);

  glUseProgram(render_info.program_id);

  // vertex position
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, render_info.vertex_buffer_object);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // uv
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, render_info.uv_buffer_object);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // colors
  glEnableVertexAttribArray(2);
  glBindBuffer(GL_ARRAY_BUFFER, render_info.color_buffer_object);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // element buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render_info.element_buffer_object);

  // texture
  glActiveTexture(GL_TEXTURE0);
  glUniform1i(render_info.texture_id, 0);

  for (u32 i = 0; i < render_info.draw_texture.size(); i++) {
    auto index = new_order[i];

    auto tex = render_info.texture[render_info.draw_texture[index]];
    auto start = render_info.draw_start_element[index];
    auto count = render_info.draw_count_element[index];

    // texture
    glBindTexture(GL_TEXTURE_2D, tex);

    // draw call
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)(intptr_t)(start * sizeof(GLuint)));
    //glDrawElementsBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0, start ? 4 : 0);
  }

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  // 
  debug::render(render_info.window);

  SDL_GL_SwapWindow(render_info.window);

  start_new_frame();
}

}
