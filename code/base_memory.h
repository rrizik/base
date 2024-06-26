#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

#include <stdlib.h>
#include "base_types.h"
#include "base_math.h"

// UNTESTED: this entire file
static void*
memory_set(void *base, int value, u32 size) {
    u8* base_ref = (u8*)base;
    while(size--) *base_ref++ = (u8)value;

    return(base);
}

static void*
memory_set(void *base, int value, u64 size) {
    u8* base_ref = (u8*)base;
    while(size--) *base_ref++ = (u8)value;

    return(base);
}

static void
memory_copy(void *dst, void *src, u32 size) {
    u8* d = (u8*)dst;
    u8* s = (u8*)src;
    while(size--) {
        *d++ = *s++;
    }
}

static void
memory_copy(void *dst, void *src, u64 size) {
    u8* d = (u8*)dst;
    u8* s = (u8*)src;
    while(size--) {
        *d++ = *s++;
    }
}

///////////////////////////////
// NOTE: Arena Functions
///////////////////////////////

typedef struct Arena{
    void* base;
    u32 size;
    u32 at;
} Arena;

// CONSIDER: not sure if I need this, if I have os_make_arena()
static Arena* make_arena(u32 size){
    void* memory = calloc((size + sizeof(Arena)), 1); // 0 initialized
    Arena* result = (Arena*)memory;
    result->base = (u8*)memory + sizeof(Arena);
    result->size = size;
    result->at = 0;
    return(result);
}

#define init_arena(a, b, s) arena_init(a, b, (u32)s)
static void arena_init(Arena* arena, void* base, u32 size){
    arena->base = base;
    arena->size = size;
    arena->at = 0;
}

static void arena_free(Arena* arena){
    memory_set(arena->base, 0, arena->at);
    arena->at = 0;
}

#define push_array(arena, type, count) (type*)push_size_aligned((arena), (u32)(sizeof(type) * (u32)(count)), alignof(type))
#define push_struct(arena, type) (type*)push_size_aligned((arena), sizeof(type), alignof(type))
static void* push_size_aligned(Arena* arena, u32 size, u32 align){
    u32 used_aligned = AlignUpPow2(arena->at, align);
    assert((used_aligned + size) <= arena->size);
    void* result = (u8*)arena->base + used_aligned;
    arena->at = used_aligned + size;
    return(result);
}

//UNTESTED:
#define pop_array(arena, type, count) pop_array_((arena), sizeof(type) * (count))
static void pop_array_(Arena* arena, u32 size){
    arena->at = arena->at - size;
}

static Arena* push_arena(Arena *arena, u32 size){
    Arena* result = push_struct(arena, Arena);
    result->base = push_array(arena, u8, size);
    result->size = size;
    result->at = 0;
    return(result);
}

typedef struct ScratchArena{
    Arena* arena;
    u32 at;
} ScratchArena;

#define DEFAULT_RESERVE_SIZE GB(1)
#define SCRATCH_POOL_COUNT 2
global THREAD_LOCAL Arena* scratch_pool[SCRATCH_POOL_COUNT] = {};
static u32 scratch_index = 0;

static ScratchArena get_scratch(Arena* arena){
    ScratchArena result;
    result.arena = arena;
    result.at = arena->at;
    return(result);
}

static ScratchArena
begin_scratch(){
    // init on first time
    static s32 index = 0;
    if (scratch_pool[0] == 0){
        Arena **scratch_slot = scratch_pool;
        for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
            Arena* arena = make_arena(DEFAULT_RESERVE_SIZE);
            *scratch_slot = arena;
        }
    }
    ScratchArena result = get_scratch((*(scratch_pool + (scratch_index % 2))));
    scratch_index++;
    return(result);
}

static void end_scratch(ScratchArena scratch){
    scratch.arena->at = scratch.at;
}

//TODO: arena_resize_align

#endif
