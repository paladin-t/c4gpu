#ifndef __C4G_CONTEXT_WIN_H__
#define __C4G_CONTEXT_WIN_H__

#include "c4g_runtime.h"

#ifdef C4G_RUNTIME_CP_VC

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

#endif /* C4G_RUNTIME_CP_VC */

#endif /* __C4G_CONTEXT_WIN_H__ */
