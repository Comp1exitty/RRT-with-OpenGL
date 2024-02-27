// version 330 core
layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;

out vec3 fragColor;
uniform mat4 transform;

void main() {
  gl_Position = transform*vec4(position, 0, 1.0);
  fragColor = color;
}