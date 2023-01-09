#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdbool.h> // NOTE: C header


///////////////////////////////
// NOTE: Context
///////////////////////////////

// Compiler
#if defined(__clang__)
# define COMPILER_CLANG 1
#elif defined(__GNUC__)
# define COMPILER_GCC 1
#elif defined(_MSC_VER)
# define COMPILER_CL 1
#endif

// OS
#if defined(_WIN32)
# define OS_WIN 1
#elif defined(__APPLE_) && defined(__MACH__)
# define OS_MAC 1
#elif defined(__gnu_linux__)
# define OS_LINUX 1
#elif defined(_MSC_VER)
# define COMPILER_CL 1
#endif

// Archatecture
#if defined(_M_AMD64) || defined(__amd64__)
# define ARCH_AMD64 1
#elif defined(_M_I86) || defined(__i386)
# define ARCH_X86
#elif defined(_M_ARM) || defined(__arm__)
# define ARCH_ARM
#endif

// C/C++
#if defined(__cplusplus)
# define STANDARD_CPP 1
#else
# define STANDARD_C 1
#endif


///////////////////////////////
// NOTE: Helper Macros
///////////////////////////////

#define ENABLE_ASSERT 1
#if ENABLE_ASSERT
# define ASSERT(cond) do { if (!(cond)) __debugbreak(); } while (0)
# define ASSERT_HR(hr) ASSERT(SUCCEEDED(hr))
# define Assert(cond) do { if (!(cond)) __debugbreak(); } while (0)
# define AssertHr(hr) Assert(SUCCEEDED(hr))
# define assert(cond) do { if (!(cond)) __debugbreak(); } while (0)
# define assert_hr(hr) assert(SUCCEEDED(hr))
#else
# define ASSERT(cond)
# define ASSERT_HR(cond)
# define Assert(cond)
# define AssertHr(cond)
# define assert(cond)
# define assert_hr(cond)
#endif

#define ArrayCount(x) (sizeof(x)/sizeof(*(x)))
#define ArrayLength(x) ArrayCount(x)
#define array_count(x) (sizeof(x)/sizeof(*(x)))
#define array_length(x) ArrayCount(x)

#if STANDARD_CPP
    #define ZERO_INIT {}
#else
    #define ZERO_INIT {0}
#endif

#define STR_(x) #x
#define STR(x) STR_(x)
#define GLUE(a,b) a##b

#define KB(x) (x * 1024LL)
#define MB(x) (KB(x) * 1024LL)
#define GB(x) (MB(x) * 1024LL)
#define TB(x) (GB(x) * 1024LL)

#define HUNDRED(x) ((x) * 100)
#define THOUSAND(x) ((x) * 1000)
#define MILLION(x) ((x) * 1000000llu)
#define BILLION(x) ((x) * 1000000000llu)
#define TRILLION(x) ((x) * 1000000000000llu)

#define global static
#define local static

///////////////////////////////
// NOTE: Basic Types
///////////////////////////////

#include <stdint.h>
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef wchar_t wchar;


// create your own bool?

///////////////////////////////
// NOTE: Basic Constants
///////////////////////////////

global s8  s8_min  = (s8) 0x80;
global s16 s16_min = (s16)0x8000;
global s32 s32_min = (s32)0x80000000;
global s64 s64_min = (s64)0x8000000000000000llu;

global s8  s8_max  = (s8) 0x7f;
global s16 s16_max = (s16)0x7fff;
global s32 s32_max = (s32)0x7fffffff;
global s64 s64_max = (s64)0x7fffffffffffffffllu;

global u8  u8_max  = (u8) 0xff;
global u16 u16_max = (u16)0xffff;
global u32 u32_max = (u32)0xffffffff;
global u64 u64_max = (u64)0xffffffffffffffffllu;

#endif
