/*
 ** C4GPU.
 **
 ** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
 **
 ** Copyright (C) 2017 Wang Renxin. All rights reserved.
 */

#include "c4g_context_macos.h"
#include <GLKit/GLKit.h>

#ifdef C4G_RUNTIME_OS_MAC

namespace c4g {

namespace gl {

struct Context {
	CGLContextObj _context = nullptr;
	CGLContextObj _oldContext = nullptr;
};

Context* createContext(void) {
	Context* result = new Context();

	CGLPixelFormatAttribute pixelFormatAttributes[] = {
		kCGLPFAOpenGLProfile, (CGLPixelFormatAttribute)kCGLOGLPVersion_3_2_Core,
		kCGLPFAColorSize, (CGLPixelFormatAttribute)24,
		kCGLPFAAlphaSize, (CGLPixelFormatAttribute)8,
		kCGLPFAAccelerated,
		(CGLPixelFormatAttribute)0
	};
	CGLPixelFormatObj pixelFormat;
	GLint numberOfPixels;
	CGLChoosePixelFormat(pixelFormatAttributes, &pixelFormat, &numberOfPixels);
	CGLCreateContext(pixelFormat, 0, &result->_context);
	CGLDestroyPixelFormat(pixelFormat);
	pushContext(result);

	return result;
}

void finishCreatingContext(Context* ctx) {
	popContext(ctx);
}

void destroyContext(Context* ctx) {
	if (!ctx) return;

	CGLDestroyContext(ctx->_context);

	delete ctx;
}

void pushContext(Context* ctx) {
	if (!ctx) return;

	ctx->_oldContext = CGLGetCurrentContext();
	CGLSetCurrentContext(ctx->_context);
}

void popContext(Context* ctx) {
	if (!ctx) return;

	CGLSetCurrentContext(ctx->_oldContext);
	ctx->_oldContext = nullptr;
}

bool isContext(Context* ctx) {
	if (!ctx) return false;

	return ctx->_context == CGLGetCurrentContext();
}

}

}

#endif /* C4G_RUNTIME_OS_MAC */
