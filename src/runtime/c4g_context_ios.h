/*
 ** C4GPU.
 **
 ** For the latest info, see https://github.com/c4gpu/c4gpu_runtime/
 **
 ** Copyright (C) 2017 Wang Renxin. All rights reserved.
 */

#ifndef __C4G_CONTEXT_IOS_H__
#define __C4G_CONTEXT_IOS_H__

#include "c4g_runtime.h"

#if defined C4G_RUNTIME_OS_IOS || defined C4G_RUNTIME_OS_IOS

namespace c4g {

namespace gl {

struct Context;

C4G_RUNTIME_IMPL Context* createContext(void);
C4G_RUNTIME_IMPL void finishCreatingContext(Context* ctx);
C4G_RUNTIME_IMPL void destroyContext(Context* ctx);

C4G_RUNTIME_IMPL void pushContext(Context* ctx);
C4G_RUNTIME_IMPL void popContext(Context* ctx);

}

}

#endif /* defined C4G_RUNTIME_OS_IOS || defined C4G_RUNTIME_OS_IOS */

#endif /* __C4G_CONTEXT_IOS_H__ */
