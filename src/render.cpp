// @Todo use indexes
// @Todo multiple program_ids
// @Performance use GL_STREAM_DRAW and GL_DYNAMIC_DRAW instead of rebuilding the buffer

#include "render.h"

#include <vector>
#include <SDL.h>
#include "shaders.h"

#include "stb/stb_image.h"


GLuint vertex_array_id;
GLuint vertex_buffer_object;
GLuint color_buffer_object;
GLuint uv_buffer_object;
GLuint program_id;

std::vector<f32> vertex_buffer;
std::vector<f32> color_buffer;
std::vector<f32> uv_buffer;

GLuint texture;
GLuint texture_id;

//
// Test
//

int texture_width, texture_height;

//
// /Test
//

void load_image(const char* filename) {
  int w, h;
  unsigned char* texture_data = stbi_load(filename, &w, &h, nullptr, 4);

  if (texture_data == nullptr) {
    printf("Could not load texture: %s\n", filename);
    return;
  }

  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  stbi_image_free(texture_data);

  texture_width = w;
  texture_height = h;
}

internal
void load_shaders() {
  program_id = load_shaders("assets/shaders/regular.vs", "assets/shaders/regular.fs");
  load_image("assets/graphics/template-32x32-up.png");

  texture_id = glGetUniformLocation(program_id, "tex");
}

void setup_rendering() {
  glGenVertexArrays(1, &vertex_array_id);

  glGenBuffers(1, &vertex_buffer_object);
  glGenBuffers(1, &color_buffer_object);
  glGenBuffers(1, &uv_buffer_object);

  load_shaders();
}

void cleanup_rendering() {
  glDeleteProgram(program_id);

  glDeleteBuffers(1, &vertex_buffer_object);
  glDeleteBuffers(1, &color_buffer_object);
  glDeleteBuffers(1, &uv_buffer_object);

  glDeleteVertexArrays(1, &vertex_array_id);
}

void add_to_render(f32 x, f32 y, f32 w, f32 h, f32 z) {
  // first triangle
  vertex_buffer.push_back(x);
  vertex_buffer.push_back(y);
  vertex_buffer.push_back(z);

  vertex_buffer.push_back(x+w);
  vertex_buffer.push_back(y);
  vertex_buffer.push_back(z);

  vertex_buffer.push_back(x+w);
  vertex_buffer.push_back(y+h);
  vertex_buffer.push_back(z);

  uv_buffer.push_back(0);
  uv_buffer.push_back(1);

  uv_buffer.push_back(1);
  uv_buffer.push_back(1);

  uv_buffer.push_back(1);
  uv_buffer.push_back(0);

  color_buffer.push_back(1.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);

  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);

  color_buffer.push_back(0.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);
  color_buffer.push_back(1.0f);


  // second triangle
  vertex_buffer.push_back(x+w);
  vertex_buffer.push_back(y+h);
  vertex_buffer.push_back(z);

  vertex_buffer.push_back(x);
  vertex_buffer.push_back(y+h);
  vertex_buffer.push_back(z);

  vertex_buffer.push_back(x);
  vertex_buffer.push_back(y);
  vertex_buffer.push_back(z);

  uv_buffer.push_back(1);
  uv_buffer.push_back(0);

  uv_buffer.push_back(0);
  uv_buffer.push_back(0);

  uv_buffer.push_back(0);
  uv_buffer.push_back(1);

  color_buffer.push_back(0.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);
  color_buffer.push_back(1.0f);

  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);

  color_buffer.push_back(1.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(0.0f);
  color_buffer.push_back(1.0f);
}

void bind_buffers() {
  glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * vertex_buffer.size(),
               &vertex_buffer[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, uv_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * uv_buffer.size(),
               &uv_buffer[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, color_buffer_object);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(f32) * color_buffer.size(),
               &color_buffer[0],
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void render() {
  glBindVertexArray(vertex_array_id);

  //update_buffers();

  glUseProgram(program_id);

  glActiveTexture(GL_TEXTURE0);
  glBindBuffer(GL_TEXTURE_2D, texture);
  glUniform1i(texture_id, 0);

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

  // draw call
  glDrawArrays(GL_TRIANGLES, 0, vertex_buffer.size());

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_TEXTURE_2D, 0);
  glBindVertexArray(0);
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

