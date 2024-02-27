// version 330 core
in vec3 fragColor;
out vec4 outColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float transparent;

void main() {
  outColor = vec4(fragColor, 1.0);
}