/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
**
** The native C eval test.
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _MSC_VER
#	include <conio.h>
#	include <Windows.h>
#endif /* _MSC_VER */
#ifdef __clang__
#	include <sys/time.h>
#endif /* __clang__ */

typedef struct Vector2 {
	float x, y;
} Vector2;

typedef struct Vector4 {
	float x, y, z, w;
} Vector4;

static float vec2_length(Vector2* p) {
	return (float)sqrt(p->x * p->x + p->y * p->y);
}

static void vec2_mul_float(Vector2* p, float q) {
	p->x *= q;
	p->y *= q;
}

static void vec2_mul_vec2(Vector2* p, Vector2* q) {
	p->x *= q->x;
	p->y *= q->y;
}

static void vec4_add_vec4(Vector4* p, Vector4* q) {
	p->x += q->x;
	p->y += q->y;
	p->z += q->z;
	p->w += q->w;
}

static void vec4_mul_float(Vector4* p, float q) {
	p->x *= q;
	p->y *= q;
	p->z *= q;
	p->w *= q;
}

static void accel(Vector2* out, Vector2* p) {
	float l = 1.0f / vec2_length(p);
	*out = *p;
	vec2_mul_float(out, -1 * (l * l * l));
}

static void eval(Vector4* out, Vector4* u0, Vector4* v0) {
	Vector4 p = *u0;
	Vector4 v = *v0;
	const float dt = 0.1f;
	for (int i = 0; i < 10000; ++i) {
		Vector2 a1, a2;
		Vector2 m = { p.x, p.y };
		Vector2 n = { p.z, p.w };
		accel(&a1, &m);
		accel(&a2, &n);
		Vector4 a = { a1.x, a1.y, a2.x, a2.y };
		Vector4 _v = v;
		vec4_mul_float(&_v, dt);
		vec4_add_vec4(&p, &_v);
		vec4_mul_float(&a, dt);
		vec4_add_vec4(&v, &a);
	}
	*out = p;
}

#if defined _MSC_VER || defined __BORLANDC__
static unsigned ticks(void) {
	LARGE_INTEGER li;
	double freq = 0.0;
	unsigned ret = 0;

	QueryPerformanceFrequency(&li);
	freq = (double)li.QuadPart / 1000.0;
	QueryPerformanceCounter(&li);
	ret = (unsigned)((double)li.QuadPart / freq);

	return ret;
}
#elif defined __clang__
static unsigned ticks(void) {
	struct timespec ts;
	struct timeval now;
	int rv = 0;

	rv = gettimeofday(&now, 0);
	if (rv)
		return 0;

	ts.tv_sec = now.tv_sec;
	ts.tv_nsec = now.tv_usec * 1000;

	return (unsigned)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#elif defined __GNUC__ || defined __GNUG__
static unsigned ticks(void) {
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (unsigned)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
#endif /* _MSC_VER || __BORLANDC__ */

int main(int argc, char* argv[]) {
	size_t len = 0;
	Vector4 un0;
	Vector4* in0 = NULL;
	Vector4* out0 = NULL;
	size_t i = 0;
	unsigned t = 0;

	printf("Native C Eval\n");

	// Prepares.
	printf("Preparing...\n");
	len = 1000000;
	un0.x = 100; un0.y = 100; un0.z = 100; un0.w = 100;
	in0 = (Vector4*)malloc(sizeof(Vector4) * len);
	for (i = 0; i < len; ++i) {
		Vector4 v = { 1, 2, 3, 4 };
		in0[i] = v;
	}
	out0 = (Vector4*)malloc(sizeof(Vector4) * len);
	memset(out0, 0, sizeof(Vector4) * len);

	// Computes.
	printf("Begin compute...\n");
	t = ticks();
	for (i = 0; i < len; ++i) {
		eval(&out0[i], &un0, &in0[i]);
	}
	printf("%dms cost.\n", ticks() - t);

	// Done.
	printf("Done.\n");
	free(in0);
	free(out0);

	getchar();

	return 0;
}
