#if !defined(BASE_TYPES_H)
#define BASE_TYPES_H

#include <stdint.h>
#include <stdbool.h>

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
# define Assert(x) if(!(x)) __debugbreak()
#else
# define Assert(x)
#endif

#define ArrayCount(x) (sizeof(x)/sizeof(*(x)))

#define MIN(a,b) (((a)<=(b))?(a):(b))
#define MAX(a,b) (((a)>=(b))?(a):(b))
#define CLAMP(a,x,b) (((x)<(a))?(a):((x)>(b))?(b):(x))
#define ABS(x) ((x)<0?-(x):(x))

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
//#define function static

// TODO: memory zero (struct, array)
// TODO: memory copy (struct, array)

///////////////////////////////
// NOTE: Basic Types
///////////////////////////////

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef wchar_t wchar;

typedef float f32;
typedef double f64;

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

global f32 f32_pi = 3.14159265359f;
global f64 f64_pi = 3.14159265359;
global f32 RAD = 0.0174533f;

///////////////////////////////
// NOTE: Compound Types
///////////////////////////////

typedef union v2{
    struct{ f32 x; f32 y; };
    struct{ f32 w; f32 h; };
    f32 v[2];
} v2;

typedef union v3{
    struct{ f32 x; f32 y; f32 z; };
    struct{ f32 r; f32 g; f32 b; };
    f32 v[3];
} v3;

typedef union v4{
    struct{ f32 x; f32 y; f32 z; f32 w; };
    struct{ f32 r; f32 g; f32 b; f32 a; };
    f32 v[4];
} v4;

typedef union v2s32{
    struct{ s32 x; s32 y; };
    struct{ s32 w; s32 h; };
    s32 v[2];
} v2s32;

///////////////////////////////
// NOTE: Compound Types Constructors
///////////////////////////////

static v2 vec2(f32 x, f32 y){
    v2 result = {x, y};
    return(result);
}

static v3 vec3(f32 x, f32 y, f32 z){
    v3 result = {x, y, z};
    return(result);
}

static v4 vec4(f32 x, f32 y, f32 w, f32 h){
    v4 result = {x, y, w, h};
    return(result);
}

static v2s32 vec2s32(s32 x, s32 y){
    v2s32 result = {x, y};
    return(result);
}

///////////////////////////////
// NOTE: Compound Types Operators
///////////////////////////////

#if STANDARD_CPP
static v2s32 operator+(const v2s32& a, const v2s32& b){
    v2s32 result = {a.x + b.x, a.y + b.y};
    return(result);
}

static v2 operator+(const v2& a, const v2& b){
    v2 result = {a.x + b.x, a.y + b.y};
    return(result);
}

static v3 operator+(const v3& a, const v3& b){
    v3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return(result);
}

static v4 operator+(const v4& a, const v4& b){
    v4 result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return(result);
}

static v2s32 operator-(const v2s32& a, const v2s32& b){
    v2s32 result = {a.x - b.x, a.y - b.y};
    return(result);
}

static v2 operator-(const v2& a, const v2& b){
    v2 result = {a.x - b.x, a.y - b.y};
    return(result);
}

static v3 operator-(const v3& a, const v3& b){
    v3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return(result);
}

static v4 operator-(const v4& a, const v4& b){
    v4 result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return(result);
}

static v2s32 operator*(const v2s32& a, const s32 b){
    v2s32 result = {a.x * b, a.y * b};
    return(result);
}

static v2 operator*(const v2& b, const f32& a){
    v2 result = {b.x * a, b.y * a};
    return(result);
}

static v3 operator*(const v3& b, const f32& a){
    v3 result = {b.x * a, b.y * a, b.z * a};
    return(result);
}

static v4 operator*(const v4& b, const f32& a){
    v4 result = {b.x * a, b.y * a, b.z * a, b.w * a};
    return(result);
}

static v2s32 operator*(const s32& a, const v2s32& b){
    v2s32 result = {b.x * a, b.y * a};
    return(result);
}

static v2 operator*(const f32& a, const v2& b){
    v2 result = {b.x * a, b.y * a};
    return(result);
}

static v3 operator*(const f32& a, const v3& b){
    v3 result = {b.x * a, b.y * a, b.z * a};
    return(result);
}

static v4 operator*(const f32& a, const v4& b){
    v4 result = {b.x * a, b.y * a, b.z * a, b.w * a};
    return(result);
}

static bool operator==(const v2s32& a, const v2s32& b){
    return((a.x == b.x) && (a.y == b.y));
}

static bool operator==(const v2& a, const v2& b){
    return((a.x == b.x) && (a.y == b.y));
}

static bool operator==(const v3& a, const v3& b){
    return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

static bool operator==(const v4& a, const v4& b){
    return((a.x == b.x) && (a.y == b.y && (a.z == b.z) && (a.w == b.w)));
}

static bool operator!=(const v2s32& a, const v2s32& b){
    return(!(a == b));
}

static bool operator!=(const v2& a, const v2& b){
    return(!(a == b));
}

static bool operator!=(const v3& a, const v3& b){
    return(!(a == b));
}

static bool operator!=(const v4& a, const v4& b){
    return(!(a == b));
}
#endif

#endif
