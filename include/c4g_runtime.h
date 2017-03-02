/*
** C4GPU.
**
** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
**
** Copyright (C) 2017 Wang Renxin. All rights reserved.
*/

#ifndef __C4G_RUNTIME_H__
#define __C4G_RUNTIME_H__

#include <stdint.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if defined _MSC_VER
#	define C4G_RUNTIME_CP_VC _MSC_VER
#elif defined __clang__
#	define C4G_RUNTIME_CP_CLANG
#elif defined __CYGWIN__
#	define C4G_RUNTIME_CP_CYGWIN
#elif defined __MINGW32__
#	define C4G_RUNTIME_CP_MINGW32
#elif defined __BORLANDC__
#	define C4G_RUNTIME_CP_BORLANDC
#elif defined __POCC__
#	define C4G_RUNTIME_CP_PELLESC
#elif defined __GNUC__ || defined __GNUG__
#	define C4G_RUNTIME_CP_GCC
#elif defined __ICC || defined __INTEL_COMPILER
#	define C4G_RUNTIME_CP_ICC
#elif defined __HP_cc || defined __HP_aCC
#	define C4G_RUNTIME_CP_HPC
#elif defined __IBMC__ || defined __IBMCPP__
#	define C4G_RUNTIME_CP_IBMC
#elif defined __PGI
#	define C4G_RUNTIME_CP_PGCC
#elif defined __SUNPRO_C || defined__SUNPRO_CC
#	define C4G_RUNTIME_CP_SOLARISC
#else
#	define C4G_RUNTIME_CP_UNKNOWN
#endif /* Compiler dependent macro */

#if defined _WIN64
#	define C4G_RUNTIME_OS_WIN
#	define C4G_RUNTIME_OS_WIN64
#elif defined _WIN32
#	define C4G_RUNTIME_OS_WIN
#	define C4G_RUNTIME_OS_WIN32
#elif defined __APPLE__
#	include <TargetConditionals.h>
#	define C4G_RUNTIME_OS_APPLE
#	if defined TARGET_OS_IPHONE && TARGET_OS_IPHONE == 1
#		define C4G_RUNTIME_OS_IOS
#	elif defined TARGET_IPHONE_SIMULATOR && TARGET_IPHONE_SIMULATOR == 1
#		define C4G_RUNTIME_OS_IOS_SIM
#	elif defined TARGET_OS_MAC && TARGET_OS_MAC == 1
#		define C4G_RUNTIME_OS_MAC
#	endif
#elif defined __linux__
#	define C4G_RUNTIME_OS_LINUX
#elif defined __unix__
#	define C4G_RUNTIME_OS_UNIX
#elif defined __ANDROID__
#	define C4G_RUNTIME_OS_ANDROID
#else
#	define C4G_RUNTIME_OS_UNKNOWN
#endif /* OS dependent macro */

#ifndef C4G_RUNTIME_API
#	ifdef _MSC_VER
#		ifdef RUNTIME_EXPORTS
#			define C4G_RUNTIME_API __declspec(dllexport)
#		else /* RUNTIME_EXPORTS */
#			define C4G_RUNTIME_API __declspec(dllimport)
#		endif /* RUNTIME_EXPORTS */
#	else /* _MSC_VER */
#		define C4G_RUNTIME_API __attribute__ ((visibility("default")))
#	endif /* _MSC_VER */
#endif /* C4G_RUNTIME_API */

#ifndef C4G_RUNTIME_IMPL
#	ifdef _MSC_VER
#		define C4G_RUNTIME_IMPL
#	else /* _MSC_VER */
#		define C4G_RUNTIME_IMPL __attribute__ ((visibility("hidden")))
#	endif /* _MSC_VER */
#endif /* C4G_RUNTIME_IMPL */

#ifndef C4G_RUNTIME_INLINE
#	define C4G_RUNTIME_INLINE inline
#endif /* C4G_RUNTIME_INLINE */

/**
 * @brief C4GPU boolean.
 */
typedef unsigned char C4GRT_Bool;

/**
 * @brief C4GPU (unsigned) byte.
 */
typedef unsigned char C4GRT_Byte;

/**
 * @brief C4GPU pass ID, zero means invalid.
 */
typedef unsigned C4GRT_PassId;

/**
 * @brief C4GPU vec2.
 */
typedef struct C4GRT_Vec2 {
	float a, b;
} C4GRT_Vec2;

/**
 * @brief C4GPU vec3.
 */
typedef struct C4GRT_Vec3 {
	float a, b, c;
} C4GRT_Vec3;

/**
 * @brief C4GPU vec4.
 */
typedef struct C4GRT_Vec4 {
	float a, b, c, d;
} C4GRT_Vec4;

/**
 * @brief C4GPU vec2i.
 */
typedef struct C4GRT_Vec2i {
	int a, b;
} C4GRT_Vec2i;

/**
 * @brief C4GPU vec3i.
 */
typedef struct C4GRT_Vec3i {
	int a, b, c;
} C4GRT_Vec3i;

/**
 * @brief C4GPU vec4i.
 */
typedef struct C4GRT_Vec4i {
	int a, b, c, d;
} C4GRT_Vec4i;

/**
 * @brief C4GPU vec2b.
 */
typedef struct C4GRT_Vec2b {
	C4GRT_Byte a, b;
} C4GRT_Vec2b;

/**
 * @brief C4GPU vec3b.
 */
typedef struct C4GRT_Vec3b {
	C4GRT_Byte a, b, c;
} C4GRT_Vec3b;

/**
 * @brief C4GPU vec4b.
 */
typedef struct C4GRT_Vec4b {
	C4GRT_Byte a, b, c, d;
} C4GRT_Vec4b;

/**
 * @brief C4GPU data types.
 */
typedef enum C4GRT_DataTypes {
	DT_INVALID,
	DT_REAL,
	DT_VEC2,
	DT_VEC3,
	DT_VEC4,
	DT_INTEGER,
	DT_VEC2I,
	DT_VEC3I,
	DT_VEC4I,
	DT_BYTE,
	DT_VEC2B,
	DT_VEC3B,
	DT_VEC4B,
	DT_USERTYPE,
	DT_COUNT
} C4GRT_DataTypes;

/**
 * @brief C4GPU data structure.
 */
typedef struct C4GRT_Data {
	C4GRT_DataTypes _type;
	union {
		unsigned char* _bytes;
		const float* _real;
		const C4GRT_Vec2* _vec2;
		const C4GRT_Vec3* _vec3;
		const C4GRT_Vec4* _vec4;
		const int* _integer;
		const C4GRT_Vec2i* _vec2i;
		const C4GRT_Vec3i* _vec3i;
		const C4GRT_Vec4i* _vec4i;
		const C4GRT_Byte* _byte;
		const C4GRT_Vec2b* _vec2b;
		const C4GRT_Vec3b* _vec3b;
		const C4GRT_Vec4b* _vec4b;
		const intptr_t _usertype;
	};
	size_t _count;
	size_t _sizePerElement;
	const char* _name;
} C4GRT_Data;

/**
 * @brief C4GPU texture structure, derived from C4GRT_Data.
 */
typedef struct C4GRT_Tex {
	C4GRT_Data _data;
	size_t _sizes[3];
	size_t _sizeCount;
} C4GRT_Tex;

/**
 * @brief C4GPU runtime structure.
 */
struct C4GRT_Runtime;

/**
 * @brief C4GPU return states.
 */
typedef enum C4GRT_States {
	ST_OK,
	ST_INVALID_ARGUMENT,
	ST_CONTEXT_ACTIVED,
	ST_CONTEXT_NOT_ACTIVED,
	ST_ERROR_OCCURED
} C4GRT_States;

/**
 * @brief C4GPU runtime error handler.
 */
typedef void (* C4GRT_ErrorHandler)(struct C4GRT_Runtime*, C4GRT_PassId, const char* const);

/**
 * @brief Opens a C4GPU runtime.
 *
 * @return - A C4GPU runtime instance.
 */
C4G_RUNTIME_API struct C4GRT_Runtime* c4grt_open(void);
/**
 * @brief Closes a C4GPU runtime.
 *
 * @param[in] rt - A C4GPU runtime instance.
 */
C4G_RUNTIME_API void c4grt_close(struct C4GRT_Runtime* rt);

/**
 * @brief Actives a C4GPU runtime context, to begin processing.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_begin(struct C4GRT_Runtime* rt);
/**
 * @brief Deactives a C4GPU runtime context, to end processing.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_end(struct C4GRT_Runtime* rt);

/**
 * @brief Sets an error handler of a runtime context.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] callback - Error handler.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_set_error_handler(struct C4GRT_Runtime* rt, C4GRT_ErrorHandler callback);
/**
 * @brief Shows informations about the driver of a runtime context.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_show_driver_info(struct C4GRT_Runtime* rt);

/**
 * @brief Adds a computation pass.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] prev - A previous pass ID, optional.
 * @return - The ID of added pass.
 */
C4G_RUNTIME_API C4GRT_PassId c4grt_add_pass(struct C4GRT_Runtime* rt, C4GRT_PassId prev);
/**
 * @brief Sets a following pass of a pass.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] next - A following pass ID.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_set_pass_flow(struct C4GRT_Runtime* rt, C4GRT_PassId pass, C4GRT_PassId next);
/**
 * @brief Sets whether to enable a data pipeline from a pass to the following.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] pipe - Whether the pipe will be enabled.
 * @param[in] pars - A string array of the output name/input name key-values for pipline flow.
 * @param[in] ps - Count of the elements in the array `pars`.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_set_pass_pipe(struct C4GRT_Runtime* rt, C4GRT_PassId pass, C4GRT_Bool pipe, const char* const * const pars, size_t ps);

/**
 * @brief Uses a GPU program file for computation.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] f - A GPU program file path.
 * @param[in] varyings - Varying names.
 * @param[in] vs - Count of varying names.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_use_gpu_program_file(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const char* const f, const char* const varyings[], size_t vs);
/**
 * @brief Uses a GPU program text for computation.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] c - A GPU program text.
 * @param[in] varyings - Varying names.
 * @param[in] vs - Count of varying names.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_use_gpu_program_string(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const char* const c, const char* const varyings[], size_t vs);

/**
 * @brief Prepares a computation, and generates GPU buffers.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] ts - Texture data buffer count.
 * @param[in] is - Input data buffer count.
 * @param[in] os - Output data buffer count.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_prepare_buffers(struct C4GRT_Runtime* rt, C4GRT_PassId pass, size_t ts, size_t is, size_t os);
/**
 * @brief Prepares and fills texture data buffer for computation.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] pd - Texture data buffer list.
 * @param[in] ds - Texture data buffer count.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_prepare_tex(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Tex* const pd, size_t ds);
/**
 * @brief Prepares uniform data for computation.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] pd - Uniform data list.
 * @param[in] ds - Uniform data count.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_prepare_uniform(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds);
/**
 * @brief Prepares and fills input data buffer for computation.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] pd - Input data buffer list.
 * @param[in] ds - Input data buffer count.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_prepare_in(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds);
/**
 * @brief Prepares output data buffer for computation.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID.
 * @param[in] pd - Output data buffer list.
 * @param[in] ds - Output data buffer count.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_prepare_out(struct C4GRT_Runtime* rt, C4GRT_PassId pass, const C4GRT_Data* const pd, size_t ds);
/**
 * @brief Computes, the output data buffer will be filled.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] head - The first pass ID to compute from, uses the first added pass if it's zero.
 * @param[in] mapimm - Whether maps output buffer immediately.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_compute(struct C4GRT_Runtime* rt, C4GRT_PassId head /* = 0 */, C4GRT_Bool mapimm /* = false */);
/**
 * @brief Maps and fills output buffer data.
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - The pass ID to be mapped.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_map_out(struct C4GRT_Runtime* rt, C4GRT_PassId pass);
/**
 * @brief Finishes computation, and deletes GPU buffers associated with some specified pass(es).
 *
 * @param[in] rt - A C4GPU runtime instance.
 * @param[in] pass - A pass ID to finish, finishes all passes if it's zero.
 * @return - Execution state.
 */
C4G_RUNTIME_API C4GRT_States c4grt_finish(struct C4GRT_Runtime* rt, C4GRT_PassId pass /* = 0 */);

/**
 * @brief Gets the count of a data structure.
 *
 * @param[in] pd - A C4GPU data structure.
 * @return - Count.
 */
C4G_RUNTIME_API size_t c4grt_data_count(const C4GRT_Data* const pd);
/**
 * @brief Gets the size in bytes of a data structure.
 *
 * @param[in] pd - A C4GPU data structure.
 * @return - Size in bytes.
 */
C4G_RUNTIME_API size_t c4grt_data_size(const C4GRT_Data* const pd);
/**
 * @brief Gets the size in bytes of an element.
 *
 * @param[in] pd - A C4GPU data structure.
 * @return - Size in bytes.
 */
C4G_RUNTIME_API size_t c4grt_data_size_per_element(const C4GRT_Data* const pd);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __C4G_RUNTIME_H__ */
