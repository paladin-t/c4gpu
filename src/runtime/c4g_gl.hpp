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

	bool open(void);
	bool close(void);

	bool begin(void);
	bool end(void);

	void showDriverInfo(void) const;

	C4GRT_PassId addPass(C4GRT_PassId prev);
	Pass* getPass(C4GRT_PassId id);

	bool compute(C4GRT_PassId head, bool mapImm = false);

	size_t finishAll(void);

	static void getDevice(char* device, size_t ds);
	static void getVersion(int* major, int* minor);

private:
	void setupGLParameters(void);

private:
	Context* _context = nullptr;
	C4GRT_PassId _headPass = 0;
	C4GRT_PassId _passIdSeed = 1;
	PassDict _passes;

};

}

}

#endif /* __C4G_GL_H__ */
