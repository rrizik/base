#ifndef BASE_TYPES_H
#define BASE_TYPES_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h> // NOTE: C header


// TODO IMPORTANT: Change all action_thing to thing_action


///////////////////////////////
// NOTE: Context
///////////////////////////////

// Compiler
#if defined(__clang__)
# define COMPILER_CLANG 1
# define COMPILER_GCC 0
# define COMPILER_CL 0
#elif defined(__GNUC__)
# define COMPILER_CLANG 0
# define COMPILER_GCC 1
# define COMPILER_CL 0
#elif defined(_MSC_VER)
# define COMPILER_CLANG 0
# define COMPILER_GCC 0
# define COMPILER_CL 1
#endif

// OS
#if defined(_WIN32)
# define OS_WINDOWS 1
#elif defined(__APPLE_)
# define OS_MAC 1
#elif defined(__linux__)
# define OS_LINUX 1
#endif

// Architecture
#if defined(_M_IX86) || defined(__i386)
# define ARCH_X86 1
#elif defined(_M_AMD64) || defined(__x86_64__) // _M_X64 is redudant with _M_AMD64
# define ARCH_X64 1
#elif defined(_M_ARM) || defined(__arm__)
# define ARCH_ARM 1
#elif defined(_M_ARM64) || defined(__aarch64__)
# define ARCH_ARM64 1
#endif

// C/C++
#if defined(__cplusplus)
# define STANDARD_CPP 1
#else
# define STANDARD_C 1
#endif

#if OS_WINDOWS
    #include <windows.h>
    #include <intrin.h>

    #if   COMPILER_CL
        #define os_debug_break() __debugbreak()
    #elif COMPILER_CLANG
        #define os_debug_break() __buildin_debugtrap()
    #else
        #define os_debug_break()
    #endif

#elif OS_LINUX
    #define os_debug_break()
#elif OS_MAC
    #define os_debug_break()
#endif

// Timestamp counter
#if defined(ARCH_ARM) || defined(ARCH_ARM64)
    #define READ_TIMESTAMP_COUNTER _ReadStatusReg(ARM64_SYSREG(3, 3, 14, 0, 2));
#elif defined(ARCH_AMD64)
    #define READ_TIMESTAMP_COUNTER __rdtsc()
#elif defined(ARCH_X64)
    #define READ_TIMESTAMP_COUNTER __rdtsc()
#elif defined(ARCH_X86)
    #define READ_TIMESTAMP_COUNTER __rdtsc()
#endif


static bool debugger_present(){
#if OS_WINDOWS
    return(IsDebuggerPresent());
#elif OS_LINUX
    return(false); // not implemented
#elif OS_MAC
    return(false); // not implemented
#else
    return(false);
#endif
}

///////////////////////////////
// NOTE: Helper Macros
///////////////////////////////

#define array_count(array) (sizeof(array) / sizeof(*(array)))

//Important: Use this as reference to improve this: https://discord.com/channels/239737791225790464/1316145567180521572
#if COMPILER_CL
    #define debug_break() do{if(debugger_present()){ os_debug_break(); }}while(0)
#elif COMPILER_CLANG
    #define debug_break() do{if(debugger_present()){ os_debug_break(); }}while(0)
#elif COMPILER_GCC
    #define debug_break() do{if(debugger_present()){ __asm__ __volatile__("int3"); }}while(0)
#else
    #define debug_break()
#endif

#ifndef ENABLE_ASSERT
#define ENABLE_ASSERT 1
#endif

#if ENABLE_ASSERT
# define assert(cond) do { if (!(cond)){ debug_break(); }} while (0)
#else
# define assert(cond)
#endif

#define invalid_code_path               assert(!(bool)"invalid_code_path");
#define invalid_default_case default: { assert(!(bool)"invalid_code_path"); } break
#define INVALID_DEFAULT_CASE invalid_default_case

#if STANDARD_CPP
    #define ZERO_INIT {}
#else
    #define ZERO_INIT {0}
#endif

#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#define GLUE_(a,b) a##b
#define GLUE(a,b) GLUE_(a,b)

#define KB(x) ((x) * 1024LL)
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

#if COMPILER_CLANG
# define THREAD_LOCAL __thread
#elif COMPILER_CL
# define THREAD_LOCAL __declspec(thread)
#else
# error no THREAD_LOCAL defined for this compiler
#endif

///////////////////////////////
// NOTE: Defer
///////////////////////////////

#if STANDARD_CPP
#if COMPILER_CLANG
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-copy-dtor"
#endif
template <typename F>
struct Defer {
    Defer(F f) : f(f) {}
    ~Defer() { f(); }
    F f;
};

template <typename F>
Defer<F> make_defer(F f) {
    return Defer<F>(f);
}

#define DO_STRING_JOIN2(arg1, arg2) arg1 ## arg2
#define STRING_JOIN2(arg1, arg2) DO_STRING_JOIN2(arg1, arg2)
#define defer(code) auto STRING_JOIN2(defer_, __LINE__) = make_defer([&](){(code);})

#if COMPILER_CLANG
#pragma clang diagnostic pop
#endif
#endif

#define defer_loop(start, end) for(int _i_ = ((start), 0); _i_ == 0; (_i_ += 1, (end)))

///////////////////////////////
// NOTE: Basic Types
///////////////////////////////

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

// TODO: create your own bool?

///////////////////////////////
// NOTE: Basic Constants
///////////////////////////////

global s8  s8_min  = 0x80;
//global s8  s8_min  = INT8_MIN;
global s16 s16_min = 0x8000;
//global s16 s16_min = INT16_MIN;
global s32 s32_min = 0x80000000;
global s64 s64_min = 0x8000000000000000llu;

global s8  s8_max  = 0x7f;
global s16 s16_max = 0x7fff;
global s32 s32_max = 0x7fffffff;
global s64 s64_max = 0x7fffffffffffffffllu;

global u8  u8_max  = 0xff;
global u16 u16_max = 0xffff;
global u32 u32_max = 0xffffffff;
global u64 u64_max = 0xffffffffffffffffllu;

typedef enum Month{
    Month_Jan,
    Month_Feb,
    Month_Mar,
    Month_Apr,
    Month_May,
    Month_Jun,
    Month_Jul,
    Month_Aug,
    Month_Sep,
    Month_Oct,
    Month_Nov,
    Month_Dec,
    Month_Count,
} Month;

#endif
