/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#ifndef __C4G_GLPASS_H__
#define __C4G_GLPASS_H__

#include "c4g_glsl.hpp"
#include "c4g_utils.hpp"
#include <map>

namespace c4g {

namespace gl {

class OpenGL;

class C4G_RUNTIME_IMPL Pass final {

private:
	typedef std::map<std::string, std::string> PipeNameDict;

public:
	Pass(OpenGL* owner, C4GRT_PassId id);
	~Pass();

	const C4GRT_PassId &id(void) const;
	C4GRT_PassId &id(void);

	const C4GRT_PassId &next(void) const;
	C4GRT_PassId &next(void);
	Pass &next(C4GRT_PassId n);
	const Pass* nextPass(void) const;
	Pass* nextPass(void);

	bool getPipe(void) const;
	bool setPipe(bool p, const char* const * const pars, size_t ps);

	bool use(Program &&prog);
	bool prepareBuffers(size_t ts, size_t is, size_t os, const SimpleErrorHandler &&callback);
	bool prepareTex(const C4GRT_Tex* const pd, size_t ds, const SimpleErrorHandler &&callback);
	bool prepareUniform(const C4GRT_Data* const pd, size_t ds, const SimpleErrorHandler &&callback);
	bool prepareIn(const C4GRT_Data* const pd, size_t ds, const SimpleErrorHandler &&callback);
	bool prepareIn(BufferList &bd, const PipeNameDict &pipes, const SimpleErrorHandler &&callback);
	bool prepareOut(const C4GRT_Data* const pd, size_t ds, const SimpleErrorHandler &&callback);
	bool compute(bool mapImm, const SimpleErrorHandler &&callback);
	bool mapOut(const SimpleErrorHandler &&callback);
	bool finish(const SimpleErrorHandler &&callback);

private:
	bool tryCheckError(const SimpleErrorHandler &&callback);

private:
	OpenGL* _owner = nullptr;
	C4GRT_PassId _id = 0;
	C4GRT_PassId _next = 0;
	bool _pipe = false;
	PipeNameDict _pipeNames;
	GLsizei _pipedCount = 0;
	bool _mapped = false;
	Program _computeProg;
	BufferList _tex;
	GLuint _texSlotBegin = 0;
	GLuint _vao = 0;
	BufferList _in;
	BufferList _out;

};

class C4G_RUNTIME_IMPL PassDict final : public std::map<C4GRT_PassId, Pass*> {

public:
	PassDict();
	~PassDict();

};

}

}

#endif /* __C4G_GLPASS_H__ */
