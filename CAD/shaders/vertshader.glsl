#version 410
// Vertex shader

layout (location = 0) in vec2 vertcoords_clip_vs;
layout (location = 1) in vec3 in_colour;

layout (location = 1) out vec3 out_colour;

void main() {
  gl_Position = vec4(vertcoords_clip_vs, 0.0, 1.0);
  out_colour = in_colour;
}
