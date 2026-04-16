//
// cmake configure.h.in
//
//
#define ENABLE_SOUI_CORE_LIB 0

#if ENABLE_SOUI_CORE_LIB
    #define LIB_CORE
#else
    #define DLL_CORE
#endif
