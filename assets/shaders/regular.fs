#version 330 core

in vec2 frag_uv;
in vec4 frag_color;

uniform sampler2D tex;

out vec4 out_color;

void main() {
  // output color = color of the texture at the specific UV
  // out_color = texture(tex, uv) * vec4(color, 1.0);
  out_color = frag_color * texture(tex, frag_uv.st);
}
