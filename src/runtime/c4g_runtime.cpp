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

#ifndef C4GRT_FRAG_SHADER
#	define C4GRT_FRAG_SHADER "#version 300 es\n\nvoid main() {\n}\n"
#endif /* C4GRT_FRAG_SHADER */

typedef struct C4GRT_Runtime {
	c4g::gl::OpenGL gl;
} C4GRT_Runtime;

struct C4GRT_Runtime* c4grt_open(void) {
	C4GRT_Runtime* result = new C4GRT_Runtime();
	if (result)
		result->gl.open();

	return result;
}

void c4grt_close(struct C4GRT_Runtime* rt) {
	if (!rt) return;

	rt->gl.close();
	delete rt;
}

C4GRT_States c4grt_begin(struct C4GRT_Runtime* rt) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (rt->gl.isCurrent()) return ST_CONTEXT_ACTIVED;

	rt->gl.begin();

	return ST_OK;
}

C4GRT_States c4grt_end(struct C4GRT_Runtime* rt) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	rt->gl.end();

	return ST_OK;
}

C4GRT_States c4grt_show_driver_info(struct C4GRT_Runtime* rt) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	rt->gl.showDriverInfo();

	return ST_OK;
}

C4GRT_PassId c4grt_add_pass(struct C4GRT_Runtime* rt, C4GRT_PassId prev) {
	if (!rt) return 0;

	if (!rt->gl.checkIsCurrent()) return 0;

	return rt->gl.addPass(prev);
}

C4GRT_States c4grt_set_pass_flow(struct C4GRT_Runtime* rt, C4GRT_PassId pass, C4GRT_PassId next) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* ppass = rt->gl.getPass(pass);
	if (!ppass) return ST_INVALID_ARGUMENT;
	if (next) {
		c4g::gl::Pass* npass = rt->gl.getPass(next);
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

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;
	if (pipe) {
		c4g::gl::Pass* f = rt->gl.getPass(p->next());
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

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	c4g::gl::Shader vert(c4g::gl::ST_VERT);
	vert.readFile(f);
	vert.compile();

	c4g::gl::Shader frag(c4g::gl::ST_FRAG);
	frag.readString(C4GRT_FRAG_SHADER);
	frag.compile();

	c4g::gl::Program comp;
	comp.link(std::move(vert), std::move(frag), varyings, vs);
	comp.use();

	p->use(std::move(comp));

	return ST_OK;
}

C4GRT_States c4grt_use_gpu_program_string(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const char* const c, const char* const varyings[], size_t vs) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!c) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	c4g::gl::Shader vert(c4g::gl::ST_VERT);
	vert.readString(c);
	vert.compile();

	c4g::gl::Shader frag(c4g::gl::ST_FRAG);
	frag.readString(C4GRT_FRAG_SHADER);
	frag.compile();

	c4g::gl::Program comp;
	comp.link(std::move(vert), std::move(frag), varyings, vs);
	comp.use();

	p->use(std::move(comp));

	return ST_OK;
}

C4GRT_States c4grt_prepare_buffers(struct C4GRT_Runtime* rt, C4GRT_PassId pass, size_t ts, size_t is, size_t os) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	p->prepareBuffers(ts, is, os);

	return ST_OK;
}

C4GRT_States c4grt_prepare_tex(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Tex* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	p->prepareTex(pd, ds);

	return ST_OK;
}

C4GRT_States c4grt_prepare_uniform(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!pd) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	p->prepareUniform(pd, ds);

	return ST_OK;
}

C4GRT_States c4grt_prepare_in(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!pd || !ds) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	p->prepareIn(pd, ds);

	return ST_OK;
}

C4GRT_States c4grt_prepare_out(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds) {
	if (!rt) return ST_INVALID_ARGUMENT;
	if (!pd || !ds) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	p->prepareOut(pd, ds);

	return ST_OK;
}

C4GRT_States c4grt_compute(struct C4GRT_Runtime* rt, C4GRT_PassId head, C4GRT_Bool mapimm) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	rt->gl.compute(head, !!mapimm);

	return ST_OK;
}

C4GRT_States c4grt_map_out(struct C4GRT_Runtime* rt, C4GRT_PassId pass) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	c4g::gl::Pass* p = rt->gl.getPass(pass);
	if (!p) return ST_INVALID_ARGUMENT;

	p->mapOut();

	return ST_OK;
}

C4GRT_States c4grt_finish(struct C4GRT_Runtime* rt, C4GRT_PassId pass) {
	if (!rt) return ST_INVALID_ARGUMENT;

	if (!rt->gl.checkIsCurrent()) return ST_CONTEXT_NOT_ACTIVED;

	if (pass) {
		c4g::gl::Pass* p = rt->gl.getPass(pass);
		if (!p) return ST_INVALID_ARGUMENT;

		p->finish();
	} else {
		rt->gl.finishAll();
	}

	return ST_OK;
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
