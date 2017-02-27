/*
 ** C4GPU.
 **
 ** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
 **
 ** Copyright (C) 2017 Wang Renxin. All rights reserved.
 */

#include "c4g_context_ios.h"
#include <GLKit/GLKit.h>

#if defined C4G_RUNTIME_OS_IOS || defined C4G_RUNTIME_OS_IOS

namespace c4g {

namespace gl {

struct Context {
	EAGLContext* _context = nullptr;
	EAGLContext* _oldContext = nullptr;
};

Context* createContext(void) {
	Context* result = new Context();

	result->_context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES3];
	pushContext(result);

	return result;
}

void finishCreatingContext(Context* ctx) {
	popContext(ctx);
}

void destroyContext(Context* ctx) {
	if (!ctx) return;

	ctx->_context = nullptr;

	delete ctx;
}

void pushContext(Context* ctx) {
	if (!ctx) return;

	ctx->_oldContext = [EAGLContext currentContext];
	[EAGLContext setCurrentContext: ctx->_context];
}

void popContext(Context* ctx) {
	if (!ctx) return;

	[EAGLContext setCurrentContext: ctx->_oldContext];
	ctx->_oldContext = nullptr;
}

bool isContext(Context* ctx) {
	if (!ctx) return false;

	return ctx->_context == [EAGLContext currentContext];
}

}

}

#endif /* defined C4G_RUNTIME_OS_IOS || defined C4G_RUNTIME_OS_IOS */
