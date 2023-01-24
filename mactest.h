#ifndef MACTEST_MACTEST_H
#define MACTEST_MACTEST_H

#if defined(_WIN32)
#define MACTEST_DLL_EXPORT __declspec(dllexport)
#define MACTEST_DLL_IMPORT __declspec(dllimoprt)
#else
#define MACTEST_DLL_EXPORT __attribute__((visibility("default")))
#define MACTEST_DLL_IMPORT
#endif

#if defined(MACTEST_STATIC)
#define MACTEST_EXPORT
#elif defined(MACTEST_exports)
#define MACTEST_EXPORT MACTEST_DLL_EXPORT
#else
#define MACTEST_EXPORT MACTEST_DLL_IMPORT
#endif

MACTEST_EXPORT char *hello(void);

#endif //MACTEST_MACTEST_H
