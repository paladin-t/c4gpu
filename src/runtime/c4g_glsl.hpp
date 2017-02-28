/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#ifndef __C4G_GLSL_H__
#define __C4G_GLSL_H__

#include "c4g_runtime.h"
#ifdef C4G_RUNTIME_OS_WIN
#	include <GL/glew.h>
#	include <GL/glut.h>
#elif defined C4G_RUNTIME_OS_APPLE
#	if (defined C4G_RUNTIME_OS_IOS || defined C4G_RUNTIME_OS_IOS_SIM)
#		include <OpenGLES/ES3/gl.h>
#	else
#		include <OpenGL/gl3.h>
#	endif
#endif /* C4G_RUNTIME_OS_WIN */
#include <functional>
#include <vector>

#ifndef GL_INVALID_INDEX
#	define GL_INVALID_INDEX -1
#endif /* GL_INVALID_INDEX */

namespace c4g {

namespace gl {

typedef std::vector<GLuint> GLuintArray;

typedef std::function<void (struct C4GRT_Runtime*, C4GRT_PassId, const char* const)> ErrorHandler;

typedef std::function<bool (const char* const)> SimpleErrorHandler;

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
	bool compile(const SimpleErrorHandler &&callback);

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
