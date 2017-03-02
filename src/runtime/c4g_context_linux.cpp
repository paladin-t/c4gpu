/*
 ** C4GPU.
 **
 ** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
 **
 ** Copyright (C) 2017 Wang Renxin. All rights reserved.
 */

#include "c4g_context_linux.hpp"

#ifdef C4G_RUNTIME_OS_LINUX

namespace c4g {

namespace gl {

struct Context {
	int _context = 0;
	int _oldContext = 0;
};

Context* createContext(void) {
	Context* result = new Context();

	//result->_context = CreateContext();
	pushContext(result);

	return result;
}

void finishCreatingContext(Context* ctx) {
	popContext(ctx);
}

void destroyContext(Context* ctx) {
	if (!ctx) return;

	//DestroyContext(result->_context);

	delete ctx;
}

void pushContext(Context* ctx) {
	if (!ctx) return;

	//ctx->_oldContext = GetCurrentContext();
	//SetCurrentContext(ctx->_context);
}

void popContext(Context* ctx) {
	if (!ctx) return;

	//SetCurrentContext(ctx->_oldContext);
	//ctx->_oldContext = nullptr;
}

bool isContext(Context* ctx) {
	if (!ctx) return false;

	//return ctx->_context == GetCurrentContext();
	return true;
}

}

}

#endif /* C4G_RUNTIME_OS_LINUX */
