#pragma once

#include "types.h"

void setup_rendering();
void cleanup_rendering();

void load_image(const char* filename);

void add_to_render(f32 x, f32 y, f32 w, f32 h, f32 z);
void bind_buffers();
void render();
