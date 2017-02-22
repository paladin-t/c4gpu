#version 300 es

uniform sampler2D s0;
uniform vec4 u0;
in vec4 v0;
in vec4 v1;
out vec4 o0;
out vec4 o1;

void main() {
	o0 = v0 + v1 + u0;
	o1 = texture2D(s0, vec2(1, 1)) + u0;
}
