/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#include "c4g_gl.hpp"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define C4GRT_FRAG_SHADER "#version 300 es\n\nvoid main() {\n}\n"

#define C4GRT_CONTEXT_ACTIVED_MESSAGE "The context is already actived."
#define C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE "The context is not actived."

typedef struct C4GRT_Runtime {
	c4g::gl::OpenGL _gl;
} C4GRT_Runtime;

C4G_RUNTIME_IMPL static bool _on_error(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const char* const msg) {
	auto callback = rt->_gl.getErrorHandler();
	if (callback != nullptr) {
		callback(rt, pass, msg);

		return true;
	}

	return false;
}

struct C4GRT_Runtime* c4grt_open(void) {
	C4GRT_Runtime* result = new C4GRT_Runtime();
	if (result)
		result->_gl.open(result);

	return result;
}

void c4grt_close(struct C4GRT_Runtime* rt) {
	if (!rt) return;

	rt->_gl.close();
	delete rt;
}

C4GRT_States c4grt_begin(struct C4GRT_Runtime* rt) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (rt->_gl.isCurrent()) {
		_on_error(rt, 0, C4GRT_CONTEXT_ACTIVED_MESSAGE);

		return ST_CONTEXT_ACTIVED;
	}

	rt->_gl.begin();

	return ST_OK;
}

C4GRT_States c4grt_end(struct C4GRT_Runtime* rt) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, 0, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	rt->_gl.end();

	return ST_OK;
}

C4GRT_States c4grt_set_error_handler(struct C4GRT_Runtime* rt, C4GRT_ErrorHandler callback) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, 0, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	rt->_gl.setErrorHandler(callback);

	return ST_OK;
}

C4G_RUNTIME_API C4GRT_States c4grt_set_error_handler_plusplus(struct C4GRT_Runtime* rt, const std::function<void (struct C4GRT_Runtime*, C4GRT_PassId, const char* const)> &callback) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, 0, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	rt->_gl.setErrorHandler(callback);

	return ST_OK;
}

C4GRT_States c4grt_show_driver_info(struct C4GRT_Runtime* rt) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, 0, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	rt->_gl.showDriverInfo();

	return ST_OK;
}

C4GRT_PassId c4grt_add_pass(struct C4GRT_Runtime* rt, C4GRT_PassId prev) {
	if (!rt) return 0;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, 0, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return 0;
	}

	return rt->_gl.addPass(prev);
}

C4GRT_States c4grt_set_pass_flow(struct C4GRT_Runtime* rt, C4GRT_PassId pass, C4GRT_PassId next) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* ppass = rt->_gl.getPass(pass);
	if (!ppass) return ST_INVALID_ARGUMENT;
	if (next) {
		c4g::gl::Pass* npass = rt->_gl.getPass(next);
		if (npass) ppass->next(next);
		else ppass->next(0);
	} else {
		ppass->next(0);
	}

	return ST_OK;
}

C4GRT_States c4grt_set_pass_pipe(struct C4GRT_Runtime* rt, C4GRT_PassId pass, C4GRT_Bool pipe, const char* const * const pars, size_t ps) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!pass) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;
	if (pipe) {
		c4g::gl::Pass* f = rt->_gl.getPass(p->next());
		if (!f) return ST_INVALID_ARGUMENT;
		p->setPipe(true, pars, ps);
	} else {
		p->setPipe(false, pars, ps);
	}

	return ST_OK;
}

C4GRT_States c4grt_use_gpu_program_file(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const char* const f, const char* const varyings[], size_t vs) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!f) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	c4g::gl::Shader vert(c4g::gl::ST_VERT);
	vert.readFile(f);
	vert.compile([&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	c4g::gl::Shader frag(c4g::gl::ST_FRAG);
	frag.readString(C4GRT_FRAG_SHADER);
	frag.compile([&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	c4g::gl::Program comp;
	comp.link(std::move(vert), std::move(frag), varyings, vs);
	comp.use();

	p->use(std::move(comp));

	return result;
}

C4GRT_States c4grt_use_gpu_program_string(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const char* const c, const char* const varyings[], size_t vs) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!c) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	c4g::gl::Shader vert(c4g::gl::ST_VERT);
	vert.readString(c);
	vert.compile([&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	c4g::gl::Shader frag(c4g::gl::ST_FRAG);
	frag.readString(C4GRT_FRAG_SHADER);
	frag.compile([&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	c4g::gl::Program comp;
	comp.link(std::move(vert), std::move(frag), varyings, vs);
	comp.use();

	p->use(std::move(comp));

	return result;
}

C4GRT_States c4grt_prepare_buffers(struct C4GRT_Runtime* rt, C4GRT_PassId pass, size_t ts, size_t is, size_t os) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	p->prepareBuffers(ts, is, os, [&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	return result;
}

C4GRT_States c4grt_prepare_tex(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Tex* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	p->prepareTex(pd, ds, [&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	return result;
}

C4GRT_States c4grt_prepare_uniform(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!pd) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	p->prepareUniform(pd, ds, [&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	return result;
}

C4GRT_States c4grt_prepare_in(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!pd || !ds) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	p->prepareIn(pd, ds, [&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	return result;
}

C4GRT_States c4grt_prepare_out(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!pd || !ds) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	p->prepareOut(pd, ds, [&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	return result;
}

C4GRT_States c4grt_compute(struct C4GRT_Runtime* rt, C4GRT_PassId head, C4GRT_Bool mapimm) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, head, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	rt->_gl.compute(head, !!mapimm, std::move(rt->_gl.getErrorHandler()));

	return ST_OK;
}

C4GRT_States c4grt_map_out(struct C4GRT_Runtime* rt, C4GRT_PassId pass) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	c4g::gl::Pass* p = rt->_gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	C4GRT_States result = ST_OK;

	p->mapOut([&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });

	return result;
}

C4GRT_States c4grt_finish(struct C4GRT_Runtime* rt, C4GRT_PassId pass) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->_gl.isCurrent()) {
		_on_error(rt, pass, C4GRT_CONTEXT_NOT_ACTIVED_MESSAGE);

		return ST_CONTEXT_NOT_ACTIVED;
	}

	C4GRT_States result = ST_OK;

	if (pass) {
		c4g::gl::Pass* p = rt->_gl.getPass(pass);
		if (!p) return ST_INVALID_ARGUMENT;

		p->finish([&] (const char* const msg) { result = ST_ERROR_OCCURED; return _on_error(rt, pass, msg); });
	} else {
		rt->_gl.finishAll();
	}

	return result;
}

size_t c4grt_data_count(const C4GRT_Data* const pd) {
	if (!pd) return 0;

	return pd->_count;
}

size_t c4grt_data_size(const C4GRT_Data* const pd) {
	if (!pd) return 0;

	return pd->_count * pd->_sizePerElement;
}

size_t c4grt_data_size_per_element(const C4GRT_Data* const pd) {
	if (!pd) return 0;

	return pd->_sizePerElement;
}

#ifdef __cplusplus
}
#endif /* __cplusplus */
