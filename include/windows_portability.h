#ifdef _WIN32
    #define strcasecmp _stricmp
#else
    #include <strings.h>
#endif