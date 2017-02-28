/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#ifndef __C4G_GL_H__
#define __C4G_GL_H__

#include "c4g_glpass.hpp"

namespace c4g {

namespace gl {

struct Context;

class C4G_RUNTIME_IMPL OpenGL final {

public:
	OpenGL();
	~OpenGL();

	bool open(struct C4GRT_Runtime* rt);
	bool close(void);

	bool begin(void);
	bool end(void);
	bool isCurrent(void) const;

	struct C4GRT_Runtime* getRuntime(void) const;
	void showDriverInfo(void) const;
	const ErrorHandler &getErrorHandler(void) const;
	void setErrorHandler(const ErrorHandler &callback);

	C4GRT_PassId addPass(C4GRT_PassId prev);
	Pass* getPass(C4GRT_PassId id);
	void clearPasses(void);

	bool compute(C4GRT_PassId head, bool mapImm = false, const ErrorHandler &&callback = nullptr);

	size_t finishAll(const SimpleErrorHandler &&callback = nullptr);

	static void getDevice(char* device, size_t ds);
	static void getVersion(int* major, int* minor);

private:
	void setupGLParameters(void);

private:
	struct C4GRT_Runtime* _runtime = nullptr;
	Context* _context = nullptr;
	C4GRT_PassId _headPass = 0;
	C4GRT_PassId _passIdSeed = 1;
	PassDict _passes;
	ErrorHandler _errorHandler;

};

}

}

#endif /* __C4G_GL_H__ */
