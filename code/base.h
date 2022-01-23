#if !defined(BASE_H)
#define BASE_H

#include <stdio.h>
#include <stdint.h>

///////////////////////////////
// NOTE: Helper Macros
///////////////////////////////

#define Assert(x) if(!(x)) __debugbreak()
#define ArrayCount(x) (sizeof(x)/sizeof(*(x)))

#define Min(a,b) (((a)<=(b))?(a):(b))
#define Max(a,b) (((a)>=(b))?(a):(b))
#define Clamp(a,x,b) (((x)<(a))?(a):((x)>(b))?(b):(x))

#define Stringify(x) #x
#define Glue(a,b) a##b

#define Kilobytes(x) (x * 1024LL)
#define Megabytes(x) (Kilobytes(x) * 1024LL)
#define Gigabytes(x) (Megabytes(x) * 1024LL)
#define Terabytes(x) (Gigaybtes(x) * 1024LL)

#define global static
#define local static
#define function static

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

typedef float f32;
typedef double f64;

///////////////////////////////
// NOTE: Basic Constants
///////////////////////////////

global s8  Min_s8  = (s8) 0x80;
global s16 Min_s16 = (s16)0x8000;
global s32 Min_s32 = (s32)0x80000000;
global s64 Min_s64 = (s64)0x8000000000000000llu;

global s8  Max_s8  = (s8) 0x7f;
global s16 Max_s16 = (s16)0x7fff;
global s32 Max_s32 = (s32)0x7fffffff;
global s64 Max_s64 = (s64)0x7fffffffffffffffllu;

global u8  Max_u8  = (u8) 0xff;
global u16 Max_u16 = (u16)0xffff;
global u32 Max_u32 = (u32)0xffffffff;
global u64 Max_u64 = (u64)0xffffffffffffffffllu;

global f32 PI_f32 = 3.14159265359f;
global f64 PI_f64 = 3.14159265359;
global f32 RAD = 0.0174533f;

///////////////////////////////
// NOTE: Math Functions
///////////////////////////////

#include <math.h>
function f32 abs_f32(f32 x){
    union{ f32 f; u32 u; } r;
    r.f = x;
    r.u &= 0x7fffffff;
    return(r.f);
}

function f64 abs_f64(f64 x){
    union{ f64 f; u64 u; } r;
    r.f = x;
    r.u &= 0x7fffffffffffffff;
    return(r.f);
}

function s32 abs_s32(s32 x){
    s32 result = ((x<0)?(-x):(x));
    return(result);
}

function s64 abs_s64(s64 x){
    s64 result = ((x<0)?(-x):(x));
    return(result);
}

function f32 sqrt_f32(f32 x){ return(sqrt(x)); }
function f32 sin_f32(f32 x){  return(sin(x)); }
function f32 cos_f32(f32 x){  return(cos(x)); }
function f32 tan_f32(f32 x){  return(tan(x)); }

function f64 sqrt_f64(f64 x){ return(sqrt(x)); }
function f64 sin_f64(f64 x){  return(sin(x)); }
function f64 cos_f64(f64 x){  return(cos(x)); }
function f64 tan_f64(f64 x){  return(tan(x)); }

function f32 lerp(f32 a, f32 t, f32 b){ 
    f32 x = ((a + ((b - a) * t))); 
    return(x);
}

function f32 unlerp(f32 a, f32 x, f32 b){ 
    f32 t = 0.0f;
    if(a != b){
        t = (x - a) / (b - a);
    }
    return(t);
}

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
    f32 v[3];
} v3;

typedef union v4{
    struct{ f32 x; f32 y; f32 z; f32 w; };
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

function v2 vec2(f32 x, f32 y){
    v2 result = {x, y};
    return(result);
}

function v3 vec3(f32 x, f32 y, f32 z){
    v3 result = {x, y, z};
    return(result);
}

function v4 vec4(f32 x, f32 y, f32 w, f32 h){
    v4 result = {x, y, w, h};
    return(result);
}

function v2s32 vec2s32(s32 x, s32 y){
    v2s32 result = {x, y};
    return(result);
}

///////////////////////////////
// NOTE: Doubly Linked List
///////////////////////////////

typedef struct Node{
    struct Node* next;
    struct Node* prev;
    void* data;
    u32 count;
} SLL, DLL, Node;

function void dll_push_front(Node* sentinel, Node* node){
    node->prev = sentinel;
    node->next = sentinel->next;

    node->prev->next = node;
    node->next->prev = node;
    sentinel->count++;
}

function void dll_push_back(Node* sentinel, Node* node){
    node->prev = sentinel->prev;
    node->next = sentinel;

    node->prev->next = node;
    node->next->prev = node;
    sentinel->count++;
}

function void dll_remove(Node* sentinel, Node* node){
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node; // (QUESTION)gaurd against node referncing nodes in linked list? is this even necessay?
    node->prev = node; // gaurd against node referncing nodes in linked list? is this even necessay?
    sentinel->count--;
}

function Node* dll_pop(Node* sentinel){
    Node* node = sentinel->next;
    sentinel->next = sentinel->next->next;
    node->next = node; // gaurd against node referncing nodes in linked list? is this even necessay?
    node->prev = node; // gaurd against node referncing nodes in linked list? is this even necessay?
    sentinel->count--;
    return(node);
}

///////////////////////////////
// NOTE: Arena Functions
///////////////////////////////

typedef struct Arena{
    void* base;
    size_t size;
    size_t used;
} Arena;

function void init_arena(Arena* arena, void* base, size_t size){
    arena->base = base;
    arena->size = size;
    arena->used = 0;
}

#define allocate_array(arena, count, type) void* allocate_size_(arena, count * sizeof(size))
#define allocate_struct(arena, type) void* allocate_size_(arena, sizeof(size))
#define allocate_size(arena, size) void* allocate_size_(arena, size)
function void* allocate_size_(Arena* arena, size_t size){
    if((arena->used + size) < arena->size){ __debugbreak(); }
    void* result = (u8*)arena->base + arena->used;
    arena->used = arena->used + size;
    return(result);
}

#endif
