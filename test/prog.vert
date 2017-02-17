#version 300 es

uniform vec4 u0;
in vec4 v0;
out vec4 o0;

vec2 accel(vec2 p) {
	float l = 1.0 / length(p);
	return -p * (l * l * l);
}

void main() {
	vec4 p = u0;
	vec4 v = v0;
	const float dt = 0.1;
	for (int i = 0; i < 10000; ++i) {
		vec2 a1 = accel(vec2(p.x, p.y));
		vec2 a2 = accel(vec2(p.z, p.w));
		vec4 a = vec4(a1, a2);
		p += v * dt;
		v += a * dt;
	}
	o0 = p;
}
