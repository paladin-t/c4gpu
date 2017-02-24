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
};

Context* createContext(void) {
	Context* result = new Context();

	result->_context = [[EAGLContext alloc] initWithAPI: kEAGLRenderingAPIOpenGLES3];
	/*PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	createWindow(&result->hWnd);
	result->hDC = GetDC(result->hWnd);

	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(result->hDC, &pfd);
	SetPixelFormat(result->hDC, iFormat, &pfd);

	result->hRC = wglCreateContext(result->hDC);
	pushContext(result);*/

	return result;
}

void finishCreatingContext(Context* ctx) {
	popContext(ctx);
}

void destroyContext(Context* ctx) {
	if (!ctx) return;

	/*wglDeleteContext(ctx->hRC);
	ReleaseDC(ctx->hWnd, ctx->hDC);*/

	delete ctx;
}

void pushContext(Context* ctx) {
	if (!ctx) return;

	/*ctx->hOldDC = wglGetCurrentDC();
	ctx->hOldRC = wglGetCurrentContext();
	wglMakeCurrent(ctx->hDC, ctx->hRC);*/
}

void popContext(Context* ctx) {
	if (!ctx) return;

	/*wglMakeCurrent(ctx->hOldDC, ctx->hOldRC);
	ctx->hOldDC = nullptr;
	ctx->hOldRC = nullptr;*/
}
		
}
	
}

#endif /* defined C4G_RUNTIME_OS_IOS || defined C4G_RUNTIME_OS_IOS */
