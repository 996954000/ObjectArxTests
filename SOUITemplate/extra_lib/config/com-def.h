//
// cmake configure.h.in
//
#define ENABLE_SOUI_COM_LIB 0

#if ENABLE_SOUI_COM_LIB
    #define LIB_SOUI_COM
#else
    #define DLL_SOUI_COM
#endif
