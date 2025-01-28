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
#elif defined(__APPLE_) && defined(__MACH__)
# define OS_MAC 1
#elif defined(__gnu_linux__)
# define OS_LINUX 1
#endif

// Architecture
#if defined(_M_AMD64) || defined(__amd64__)
# define ARCH_AMD64 1
#elif defined(_M_I86) || defined(__i386)
# define ARCH_X86 1
#elif defined(_M_X64) || defined(__x86_64__)
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
#endif


#if defined(ARCH_ARM) || defined(ARCH_ARM64)
    inline void enable_arm_cycle_counter() {
        _WriteStatusReg(ARM64_PMCR_EL0, (1 << 0) | (1 << 2));  // Enable counter and reset cycle counter
        _WriteStatusReg(ARM64_PMCNTENSET_EL0, (1 << 31));      // Enable cycle counter
    }
    #define READ_TIMESTAMP_COUNTER _ReadStatusReg(ARM64_PMCCNTR_EL0)
#elif defined(ARCH_AMD64)
    #define READ_TIMESTAMP_COUNTER __rdtsc()
#elif defined(ARCH_X64)
    #define READ_TIMESTAMP_COUNTER __rdtsc()
#elif defined(ARCH_X86)
    #define READ_TIMESTAMP_COUNTER __rdtsc()
#endif


///////////////////////////////
// NOTE: Helper Macros
///////////////////////////////


#define array_count(array) (sizeof(array) / sizeof(*(array)))


#if COMPILER_CL
    #define debug_break() do{if(IsDebuggerPresent()){ __debugbreak(); }}while(0)
#elif COMPILER_CLANG
    #define debug_break() do{if(IsDebuggerPresent()){ __buildin_debugtrap(); }}while(0)
#elif COMPILER_GCC
    #define debug_break() do{if(IsDebuggerPresent()){ __asm__ __volatile__("int3"); }}while(0)
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
