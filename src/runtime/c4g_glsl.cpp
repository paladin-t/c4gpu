/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#include "c4g_glsl.hpp"
#include <string.h>

namespace c4g {

namespace gl {

Shader::Shader() {
}

Shader::Shader(ShaderTypes type) : _type(type) {
}

Shader::~Shader() {
	if (_code)
		free(_code);
}

bool Shader::readFile(const char* const file) {
	// Gets file length.
	FILE* fp = fopen(file, "rb");
	int l = -1;
	if (!fp)
		return false;

	fseek(fp, 0, SEEK_END);
	l = (int)(ftell(fp) + 1);

	// Reads code.
	_code = (GLchar*)malloc(sizeof(GLchar) * l);

	// Get the shader from a file.
	fseek(fp, 0, SEEK_SET);
	l = (int)fread(_code, sizeof(GLchar), l, fp);
	_code[l] = '\0';

	if (ferror(fp))
		l = 0;

	fclose(fp);

	if (!l) {
		printf("Cannot read the file %s.\n", file);

		return false;
	}

	return true;
}

bool Shader::readString(const char* const str) {
	if (!str)
		return false;

#ifdef C4G_RUNTIME_OS_WIN
	_code = _strdup(str);
#else /* C4G_RUNTIME_OS_WIN */
	_code = strdup(str);
#endif /* C4G_RUNTIME_OS_WIN */

	return true;
}

bool Shader::compile(const SimpleErrorHandler &&callback) {
	if (!_code) return false;

	GLint status = 0;

	switch (_type) {
	case ST_VERT:
		_object = glCreateShader(GL_VERTEX_SHADER);

		break;
	case ST_FRAG:
		_object = glCreateShader(GL_FRAGMENT_SHADER);

		break;
	default:
		return false;
	}

	glShaderSource(_object, 1, &_code, nullptr);

	glCompileShader(_object);
	glGetShaderiv(_object, GL_COMPILE_STATUS, &status);

	if (status != GL_TRUE) {
		char errorLog[1024];
		GLsizei len = 0;
		glGetShaderInfoLog(_object, sizeof(errorLog), &len, errorLog);
		if (callback != nullptr)
			callback(errorLog);
		else
			printf("%s\n", errorLog);

		return false;
	}

	return true;
}

GLuint Shader::object(void) {
	return _object;
}

Program::Program() {
}

Program::~Program() {
	if (_prog) {
		glDeleteProgram(_prog);
		_prog = 0;
	}
}

bool Program::link(Shader &&vert, Shader &&frag) {
	GLint linked = 0;

	std::swap(_vert, vert);
	std::swap(_frag, frag);

	_prog = glCreateProgram();
	glAttachShader(_prog, _vert.object());
	glAttachShader(_prog, _frag.object());

	glLinkProgram(_prog);
	glGetProgramiv(_prog, GL_LINK_STATUS, &linked);

	glDetachShader(_prog, _vert.object());
	glDetachShader(_prog, _frag.object());
	glDeleteShader(_vert.object());
	glDeleteShader(_frag.object());

	if (!linked)
		return false;

	return true;
}

bool Program::link(Shader &&vert, Shader &&frag, const char* const varyings[], size_t vs) {
	GLint linked = 0;

	std::swap(_vert, vert);
	std::swap(_frag, frag);

	_prog = glCreateProgram();
	glAttachShader(_prog, _vert.object());
	glAttachShader(_prog, _frag.object());

	glTransformFeedbackVaryings(_prog, (GLsizei)vs, varyings, GL_SEPARATE_ATTRIBS);

	glLinkProgram(_prog);
	glGetProgramiv(_prog, GL_LINK_STATUS, &linked);

	glDetachShader(_prog, _vert.object());
	glDetachShader(_prog, _frag.object());
	glDeleteShader(_vert.object());
	glDeleteShader(_frag.object());

	if (!linked)
		return false;

	return true;
}

bool Program::use(void) {
	glUseProgram(_prog);

	return true;
}

GLint Program::attributeLocation(const char* const name) {
	return glGetAttribLocation(_prog, name);
}

GLint Program::uniformLocation(const char* const name) {
	return glGetUniformLocation(_prog, name);
}

void Program::uniform(int loc, float f0) {
	glUniform1f((GLint)loc, f0);
}

void Program::uniform(int loc, float f0, float f1) {
	glUniform2f((GLint)loc, f0, f1);
}

void Program::uniform(int loc, float f0, float f1, float f2) {
	glUniform3f((GLint)loc, f0, f1, f2);
}

void Program::uniform(int loc, float f0, float f1, float f2, float f3) {
	glUniform4f((GLint)loc, f0, f1, f2, f3);
}

void Program::uniform(int loc, int i0) {
	glUniform1i((GLint)loc, i0);
}

void Program::uniform(int loc, int i0, int i1) {
	glUniform2i((GLint)loc, i0, i1);
}

void Program::uniform(int loc, int i0, int i1, int i2) {
	glUniform3i((GLint)loc, i0, i1, i2);
}

void Program::uniform(int loc, int i0, int i1, int i2, int i3) {
	glUniform4i((GLint)loc, i0, i1, i2, i3);
}

GLuint Program::object(void) {
	return _prog;
}

}

}
