/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#include "c4g_context_ios.h"
#include "c4g_context_linux.hpp"
#include "c4g_context_macos.h"
#include "c4g_context_win.hpp"
#include "c4g_gl.hpp"

namespace c4g {

namespace gl {

OpenGL::OpenGL() {
}

OpenGL::~OpenGL() {
	clearPasses();
}

bool OpenGL::open(struct C4GRT_Runtime* rt) {
	_runtime = rt;

	_context = createContext();

#ifdef C4G_RUNTIME_OS_WIN
	glewInit();
#endif /* C4G_RUNTIME_OS_WIN */

	int major, minor;
	c4g::gl::OpenGL::getVersion(&major, &minor);
	if (major < 2) {
		const char* const msg = "Support for OpenGL 2.0 is required.";
		printf("%s\n", msg);

		finishCreatingContext(_context);

		return false;
	}

	setupGLParameters();

	finishCreatingContext(_context);

	return true;
}

bool OpenGL::close(void) {
	destroyContext(_context);

	return true;
}

bool OpenGL::begin(void) {
	pushContext(_context);

	return true;
}

bool OpenGL::end(void) {
	popContext(_context);

	return true;
}

bool OpenGL::isCurrent(void) const {
	return isContext(_context);
}

struct C4GRT_Runtime* OpenGL::getRuntime(void) const {
	return _runtime;
}

void OpenGL::showDriverInfo(void) const {
	char device[256];
	memset(device, 0, sizeof(device));
	c4g::gl::OpenGL::getDevice(device, sizeof(device));
	printf("%s", device);
	int major, minor;
	c4g::gl::OpenGL::getVersion(&major, &minor);
	printf("OpenGL Version: %d.%d.\n", major, minor);
}

const ErrorHandler &OpenGL::getErrorHandler(void) const {
	return _errorHandler;
}

void OpenGL::setErrorHandler(const ErrorHandler &callback) {
	_errorHandler = callback;
}

C4GRT_PassId OpenGL::addPass(C4GRT_PassId prev) {
	C4GRT_PassId result = _passIdSeed++;
	if (!result)
		result = _passIdSeed++;

	_passes[result] = new Pass(this, result);

	Pass* ppass = getPass(prev);
	if (ppass)
		ppass->next(result);

	if (!_headPass)
		_headPass = result;

	return result;
}

Pass* OpenGL::getPass(C4GRT_PassId id) {
	auto it = _passes.find(id);
	if (it == _passes.end())
		return nullptr;

	return it->second;
}

void OpenGL::clearPasses(void) {
	for (auto it : _passes)
		delete it.second;
	_passes.clear();
}

bool OpenGL::compute(C4GRT_PassId head, bool mapImm, const ErrorHandler &&callback) {
	if (_passes.empty()) return false;

	C4GRT_PassId id = head ? head : _headPass;
	while (id) {
		Pass* p = getPass(id);
		if (!p) break;
		auto handler = [=] (const char* const msg) { if (callback != nullptr) { callback(_runtime, id, msg); return true; } return false; };
		if (!p->compute(mapImm, std::move(handler)))
			return false;
		id = p->next();
	}

	return true;
}

size_t OpenGL::finishAll(const SimpleErrorHandler &&callback) {
	size_t result = 0;
	for (auto it : _passes) {
		if (it.second->finish(std::move(callback)))
			++result;
	}

	return result;
}

void OpenGL::getDevice(char* device, size_t ds) {
	const char* vendor = (const char*)glGetString(GL_VENDOR);
	const char* renderer = (const char*)glGetString(GL_RENDERER);
	if (vendor && renderer) {
		snprintf(device, ds, "Device: %s by %s.\n", renderer, vendor);
	}
}

void OpenGL::getVersion(int* major, int* minor) {
	const char* verstr = (const char*)glGetString(GL_VERSION);
	if (!verstr || sscanf(verstr, "%d.%d", major, minor) != 2) {
		*major = *minor = 0;
		fprintf(stderr, "Invalid GL_VERSION format.\n");
	}
}

void OpenGL::setupGLParameters(void) {
#if !defined C4G_RUNTIME_OS_IOS && !defined C4G_RUNTIME_OS_IOS_SIM
	glEnable(GL_TEXTURE_1D);
#endif
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_3D);
}

}

}
