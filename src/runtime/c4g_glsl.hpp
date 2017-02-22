#ifndef __C4G_GLSL_H__
#define __C4G_GLSL_H__

#include "c4g_runtime.h"
#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>

namespace c4g {

namespace gl {

typedef std::vector<GLuint> GLuintArray;

enum ShaderTypes {
	ST_NONE,
	ST_VERT,
	ST_FRAG
};

class C4G_RUNTIME_IMPL Shader final {

public:
	Shader();
	Shader(ShaderTypes type);
	~Shader();

	bool readFile(const char* const file);
	bool readString(const char* const str);
	bool compile(void);

	GLuint object(void);

private:
	ShaderTypes _type = ST_NONE;
	GLchar* _code = nullptr;
	GLuint _object = 0;

};

class C4G_RUNTIME_IMPL Program final {

public:
	Program();
	~Program();

	bool link(Shader &&vert, Shader &&frag);
	bool link(Shader &&vert, Shader &&frag, const char* const varyings[], size_t vs);
	bool use(void);

	GLint attributeLocation(const char* const name);
	GLint uniformLocation(const char* const name);
	GLint uniformLocationARB(const char* const name);
	GLint uniformBlockIndex(const char* const name);
	void uniform(int loc, float f0);
	void uniform(int loc, float f0, float f1);
	void uniform(int loc, float f0, float f1, float f2);
	void uniform(int loc, float f0, float f1, float f2, float f3);
	void uniform(int loc, int i0);
	void uniform(int loc, int i0, int i1);
	void uniform(int loc, int i0, int i1, int i2);
	void uniform(int loc, int i0, int i1, int i2, int i3);

	GLuint object(void);

private:
	Shader _vert;
	Shader _frag;
	GLuint _prog = 0;

};

}

}

#endif /* __C4G_GLSL_H__ */
