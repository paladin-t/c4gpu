/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#include "c4g_gl.hpp"

namespace c4g {

namespace gl {

#define C4GRT_CHECK_ERROR do { if (tryCheckError(std::move(callback))) return false; } while (false)

C4G_RUNTIME_IMPL static GLenum type(C4GRT_DataTypes t) {
	switch (t) {
	case DT_REAL:
		return GL_FLOAT;
	case DT_VEC2:
	case DT_VEC3:
	case DT_VEC4:
		return GL_FLOAT;
	case DT_INTEGER:
		return GL_INT;
	case DT_VEC2I:
	case DT_VEC3I:
	case DT_VEC4I:
		return GL_INT;
	case DT_BYTE:
		return GL_BYTE;
	case DT_VEC2B:
	case DT_VEC3B:
	case DT_VEC4B:
		return GL_BYTE;
	case DT_USERTYPE:
		return GL_BYTE;
	default:
		return GL_NONE;
	}
}

C4G_RUNTIME_IMPL static GLsizei size(C4GRT_DataTypes t) {
	switch (t) {
	case DT_REAL:
		return sizeof(GLfloat);
	case DT_VEC2:
	case DT_VEC3:
	case DT_VEC4:
		return sizeof(GLfloat);
	case DT_INTEGER:
		return sizeof(GLint);
	case DT_VEC2I:
	case DT_VEC3I:
	case DT_VEC4I:
		return sizeof(GLint);
	case DT_BYTE:
		return sizeof(GLbyte);
	case DT_VEC2B:
	case DT_VEC3B:
	case DT_VEC4B:
		return sizeof(GLbyte);
	case DT_USERTYPE:
		return sizeof(GLbyte);
	default:
		return 0;
	}
}

Pass::Pass(OpenGL* owner, C4GRT_PassId id) : _owner(owner), _id(id) {
}

Pass::~Pass() {
	finish(nullptr);
}

const C4GRT_PassId &Pass::id(void) const {
	return _id;
}

C4GRT_PassId &Pass::id(void) {
	return _id;
}

const C4GRT_PassId &Pass::next(void) const {
	return _next;
}

C4GRT_PassId &Pass::next(void) {
	return _next;
}

Pass &Pass::next(C4GRT_PassId n) {
	_next = n;

	return *this;
}

const Pass* Pass::nextPass(void) const {
	if (!_owner) return nullptr;
	if (!_next) return nullptr;

	return _owner->getPass(_next);
}

Pass* Pass::nextPass(void) {
	if (!_owner) return nullptr;
	if (!_next) return nullptr;

	return _owner->getPass(_next);
}

bool Pass::getPipe(void) const {
	return _pipe;
}

bool Pass::setPipe(bool p, const char* const * const pars, size_t ps) {
	_pipe = p;
	if (_pipe && pars) {
		if (ps % 2) return false;
		_pipeNames.clear();
		for (size_t i = 0; i < ps; i += 2) {
			_pipeNames[pars[i]] = pars[i + 1];
		}
	} else {
		_pipeNames.clear();
	}

	return !!_next;
}

bool Pass::use(Program &&prog) {
	std::swap(_computeProg, prog);

	return true;
}

bool Pass::prepareBuffers(size_t ts, size_t is, size_t os, const SimpleErrorHandler &&callback) {
	// Tidies.
	finish(std::move(callback));

	// Texture buffers.
	for (size_t i = 0; i < ts; ++i) {
		_tex.push();
		Buffer &b = _tex.back();
		glActiveTexture((GLenum)(GL_TEXTURE0 + i + 1 + _texSlotBegin));
		C4GRT_CHECK_ERROR;
		glGenTextures(1, &b.id());
	}
	Pass* next = nextPass();
	if (next)
		next->_texSlotBegin = (GLuint)ts;

	// Input data buffers.
	glGenVertexArrays(1, &_vao);
	glBindVertexArray(_vao);
	for (size_t i = 0; i < is; ++i) {
		_in.push();
		Buffer &b = _in.back();
		glGenBuffers(1, &b.id());
		C4GRT_CHECK_ERROR;
	}

	// Output data buffers.
	for (size_t i = 0; i < os; ++i) {
		_out.push();
		Buffer &b = _out.back();
		glGenBuffers(1, &b.id());
		C4GRT_CHECK_ERROR;
	}

	return true;
}

bool Pass::prepareTex(const C4GRT_Tex* const pd, size_t ds, const SimpleErrorHandler &&callback) {
	_computeProg.use();
	size_t i = 0;
	GLint j = 0;
	auto it = _tex.begin();
	for (; i < ds && it != _tex.end(); ++i, ++j, ++it) {
		const C4GRT_Tex &d = pd[i];
		Buffer &b = *it;
		const C4GRT_Data* pcd = reinterpret_cast<const C4GRT_Data*>(&d);

		GLint fc = 4;
		GLenum fmt = GL_RGBA;
		GLenum type = GL_FLOAT;
		switch (pcd->_type) {
		case DT_REAL: type = GL_FLOAT; fc = 1; fmt = GL_RED; break;
		case DT_VEC2: type = GL_FLOAT; fc = 2; fmt = GL_RG; break;
		case DT_VEC3: type = GL_FLOAT; fc = 3; fmt = GL_RGB; break;
		case DT_VEC4: type = GL_FLOAT; fc = 4; fmt = GL_RGBA; break;
		case DT_INTEGER: type = GL_INT; fc = 1; fmt = GL_RED; break;
		case DT_VEC2I: type = GL_INT; fc = 2; fmt = GL_RG; break;
		case DT_VEC3I: type = GL_INT; fc = 3; fmt = GL_RGB; break;
		case DT_VEC4I: type = GL_INT; fc = 4; fmt = GL_RGBA; break;
		case DT_BYTE: type = GL_UNSIGNED_BYTE; fc = 1; fmt = GL_RED; break;
		case DT_VEC2B: type = GL_UNSIGNED_BYTE; fc = 2; fmt = GL_RG; break;
		case DT_VEC3B: type = GL_UNSIGNED_BYTE; fc = 3; fmt = GL_RGB; break;
		case DT_VEC4B: type = GL_UNSIGNED_BYTE; fc = 4; fmt = GL_RGBA; break;
		case DT_USERTYPE: type = GL_UNSIGNED_BYTE; break;
		default: break;
		}

		glActiveTexture((GLenum)(GL_TEXTURE0 + i + 1 + _texSlotBegin));
		switch (d._sizeCount) {
		case 1:
#if !defined C4G_RUNTIME_OS_IOS && !defined C4G_RUNTIME_OS_IOS_SIM
			b.map(*pcd);
			glBindTexture(GL_TEXTURE_1D, b.id());
			glTexImage1D(GL_TEXTURE_1D, 0, fc, (GLsizei)d._sizes[0], 0, fmt, type, b.ptr());
			C4GRT_CHECK_ERROR;
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
#endif

			break;
		case 2:
			b.map(*pcd);
			glBindTexture(GL_TEXTURE_2D, b.id());
			glTexImage2D(GL_TEXTURE_2D, 0, fc, (GLsizei)d._sizes[0], (GLsizei)d._sizes[1], 0, fmt, type, b.ptr());
			C4GRT_CHECK_ERROR;
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			break;
		case 3:
			b.map(*pcd);
			glBindTexture(GL_TEXTURE_3D, b.id());
			glTexImage3D(GL_TEXTURE_3D, 0, fc, (GLsizei)d._sizes[0], (GLsizei)d._sizes[1], (GLsizei)d._sizes[2], 0, fmt, type, b.ptr());
			C4GRT_CHECK_ERROR;
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

			break;
		default:
			continue;
		}

		GLint k = j;
		if (b.name()) {
			k = _computeProg.uniformLocation(b.name());
			if (k == GL_INVALID_INDEX) k = j;
		}
		_computeProg.uniform((int)k, (int)b.id());
	}

	return true;
}

bool Pass::prepareUniform(const C4GRT_Data* const pd, size_t ds, const SimpleErrorHandler &&callback) {
	if (!pd) return false;
	_computeProg.use();
	for (size_t i = 0; i < ds; ++i) {
		const C4GRT_Data &d = pd[i];
		GLint k = _computeProg.uniformLocation(d._name);
		if (k == GL_INVALID_INDEX) return false;
		switch (d._type) {
		case DT_REAL:
			_computeProg.uniform(k, *d._real); break;
		case DT_VEC2:
			_computeProg.uniform(k, d._vec2->a, d._vec2->b); break;
		case DT_VEC3:
			_computeProg.uniform(k, d._vec3->a, d._vec3->b, d._vec3->c); break;
		case DT_VEC4:
			_computeProg.uniform(k, d._vec4->a, d._vec4->b, d._vec4->c, d._vec4->d); break;
		case DT_INTEGER:
			_computeProg.uniform(k, *d._integer); break;
		case DT_VEC2I:
			_computeProg.uniform(k, d._vec2i->a, d._vec2i->b); break;
		case DT_VEC3I:
			_computeProg.uniform(k, d._vec3i->a, d._vec3i->b, d._vec3i->c); break;
		case DT_VEC4I:
			_computeProg.uniform(k, d._vec4i->a, d._vec4i->b, d._vec4i->c, d._vec4i->d); break;
		case DT_BYTE:
			_computeProg.uniform(k, *d._byte); break;
		case DT_VEC2B:
			_computeProg.uniform(k, d._vec2b->a, d._vec2b->b); break;
		case DT_VEC3B:
			_computeProg.uniform(k, d._vec3b->a, d._vec3b->b, d._vec3b->c); break;
		case DT_VEC4B:
			_computeProg.uniform(k, d._vec4b->a, d._vec4b->b, d._vec4b->c, d._vec4b->d); break;
		case DT_USERTYPE:
			_computeProg.uniform(k, *d._real); break;
		default:
			return false;
		}
	}

	return true;
}

bool Pass::prepareIn(const C4GRT_Data* const pd, size_t ds, const SimpleErrorHandler &&callback) {
	glBindVertexArray(_vao);

	_computeProg.use();
	size_t i = 0;
	GLint j = 0;
	auto it = _in.begin();
	for (; i < ds && it != _in.end(); ++i, ++j, ++it) {
		const C4GRT_Data &d = pd[i];
		Buffer &b = *it;
		b.map(d);

		glBindBuffer(GL_ARRAY_BUFFER, b.id());
		glBufferData(GL_ARRAY_BUFFER, b.size(), b.ptr(), GL_STREAM_DRAW);
		C4GRT_CHECK_ERROR;

		GLint k = j;
		if (b.name()) {
			k = _computeProg.attributeLocation(b.name());
			if (k == GL_INVALID_INDEX) k = j;
		}

		glEnableVertexAttribArray(k);
		glVertexAttribPointer(
			k, (GLint)b.sizePerElement() / size(b.type()),
			type(b.type()), GL_FALSE,
			(GLsizei)b.sizePerElement(), 0
		);
		C4GRT_CHECK_ERROR;
	}
	_pipedCount = 0;

	return true;
}

bool Pass::prepareIn(BufferList &bd, const PipeNameDict &pipes, const SimpleErrorHandler &&callback) {
	glBindVertexArray(_vao);

	_computeProg.use();
	for (Buffer &b : bd) {
		if (!b.name())
			continue;
		auto pit = pipes.find(b.name());
		if (pit == pipes.end())
			continue;

		GLint k = _computeProg.attributeLocation(pit->second.c_str());
		if (k == GL_INVALID_INDEX)
			continue;

		glBindBuffer(GL_ARRAY_BUFFER, b.id());
		C4GRT_CHECK_ERROR;

		glEnableVertexAttribArray(k);
		glVertexAttribPointer(
			k, (GLint)b.sizePerElement() / size(b.type()),
			type(b.type()), GL_FALSE,
			(GLsizei)b.sizePerElement(), 0
		);
		C4GRT_CHECK_ERROR;
	}
	if (!bd.empty())
		_pipedCount = (GLsizei)bd.front().count();
	else
		_pipedCount = 0;

	return true;
}

bool Pass::prepareOut(const C4GRT_Data* const pd, size_t ds, const SimpleErrorHandler &&callback) {
	size_t i = 0;
	auto it = _out.begin();
	for (; i < ds && it != _out.end(); ++i, ++it) {
		const C4GRT_Data &d = pd[i];
		Buffer &b = *it;
		b.map(d);

		glBindBuffer(GL_ARRAY_BUFFER, b.id());
		glBufferData(GL_ARRAY_BUFFER, b.size(), nullptr, GL_STREAM_DRAW);
		C4GRT_CHECK_ERROR;
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	return true;
}

bool Pass::compute(bool mapImm, const SimpleErrorHandler &&callback) {
	_mapped = false;

	_computeProg.use();
	glEnable(GL_RASTERIZER_DISCARD);
	{
		glBindVertexArray(_vao);

		GLuint i = 0;
		for (auto it = _out.begin(); it != _out.end(); ++i, ++it) {
			Buffer &b = *it;
			glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i, b.id());
			C4GRT_CHECK_ERROR;
		}

		glBeginTransformFeedback(GL_POINTS);
		C4GRT_CHECK_ERROR;
		{
			if (_in.empty())
				glDrawArrays(GL_POINTS, 0, _pipedCount);
			else
				glDrawArrays(GL_POINTS, 0, (GLsizei)_in.front().count());
			C4GRT_CHECK_ERROR;
		}
		glEndTransformFeedback();
		C4GRT_CHECK_ERROR;
	}
	glDisable(GL_RASTERIZER_DISCARD);
	glFinish();

	if (mapImm) {
		mapOut(std::move(callback));
	}
	if (_pipe) {
		Pass* following = nextPass();
		if (following)
			following->prepareIn(_out, _pipeNames, std::move(callback));
	}

	return true;
}

bool Pass::mapOut(const SimpleErrorHandler &&callback) {
	if (_mapped) return true;

	for (auto it = _out.begin(); it != _out.end(); ++it) {
		Buffer &b = *it;
		glBindBuffer(GL_ARRAY_BUFFER, b.id());
		C4GRT_CHECK_ERROR;
		GLchar* mem = (GLchar*)glMapBufferRange(GL_ARRAY_BUFFER, 0, b.size(), GL_MAP_READ_BIT);
		C4GRT_CHECK_ERROR;
		memcpy(b.ptr(), mem, b.size());
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}

	_mapped = true;

	return true;
}

bool Pass::finish(const SimpleErrorHandler &&callback) {
	// Output buffers.
	if (!_out.empty()) {
		glBindVertexArray(0);
		for (Buffer &b : _out) {
			if (b.id())
				glDeleteBuffers(1, &b.id());
		}
		_out.clear();
	}

	// Input buffers.
	if (!_in.empty()) {
		for (Buffer &b : _in) {
			if (b.id())
				glDeleteBuffers(1, &b.id());
		}
		_in.clear();
	}
	if (_vao) {
		glDeleteVertexArrays(1, &_vao);
		_vao = 0;
	}

	// Texture buffers.
	if (!_tex.empty()) {
		for (Buffer &b : _tex) {
			if (b.id())
				glDeleteTextures(1, &b.id());
		}
		_tex.clear();
	}

	return true;
}

bool Pass::tryCheckError(const SimpleErrorHandler &&callback) {
	GLenum err = glGetError();
	if (err == GL_NO_ERROR) return false;

	if (callback != nullptr) {
		const char* str = (const char*)gluErrorString(err);
		if (!callback(str))
			printf("%s\n", str);
	}

	return true;
}

PassDict::PassDict() {
}

PassDict::~PassDict() {
}

}

}
