#include "c4g_context_win.hpp"
#include <Windows.h>

#ifdef C4G_RUNTIME_CP_VC

namespace c4g {

namespace gl {

struct Context {
	HWND hWnd = nullptr;
	HDC hDC = nullptr;
	HGLRC hRC = nullptr;
	HDC hOldDC = nullptr;
	HGLRC hOldRC = nullptr;
};

C4G_RUNTIME_IMPL static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}

C4G_RUNTIME_IMPL static int createWindow(HWND* hWnd) {
	if (!hWnd)
		return -1;

	HINSTANCE hInstance = 0;
	MSG msg = { 0 };
	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = L"c4g";
	wc.style = CS_OWNDC;
	if (!RegisterClass(&wc))
		return 1;

	*hWnd = CreateWindowW(wc.lpszClassName, L"c4g", WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, 0, 0, hInstance, 0);

	return 0;
}

Context* createContext(void) {
	Context* result = new Context();

	PIXELFORMATDESCRIPTOR pfd;
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
	pushContext(result);

	return result;
}

void finishCreatingContext(Context* ctx) {
	popContext(ctx);
}

void destroyContext(Context* ctx) {
	if (!ctx) return;

	wglDeleteContext(ctx->hRC);
	ReleaseDC(ctx->hWnd, ctx->hDC);

	delete ctx;
}

void pushContext(Context* ctx) {
	if (!ctx) return;

	ctx->hOldDC = wglGetCurrentDC();
	ctx->hOldRC = wglGetCurrentContext();
	wglMakeCurrent(ctx->hDC, ctx->hRC);
}

void popContext(Context* ctx) {
	if (!ctx) return;

	wglMakeCurrent(ctx->hOldDC, ctx->hOldRC);
	ctx->hOldDC = nullptr;
	ctx->hOldRC = nullptr;
}

}

}

#endif /* C4G_RUNTIME_CP_VC */
