#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

#include <stdlib.h>
#include "base_types.h"
#include "base_math.h"

// UNTESTED:
static void*
mem_set(void *base, int source, size_t size) {
    u8* base_ref = (u8*)base;
    while(size--) *base_ref++ = (u8)source;

    return(base);
}

// UNTESTED:
static void*
mem_copy(void *base_to, void const *base_from, size_t size) {
    unsigned char *base_to_ref = (unsigned char *)base_to;
    unsigned char *base_from_ref = (unsigned char *)base_from;
    while(size--) *base_to_ref++ = *base_from_ref++;

    return(base_to);
}

///////////////////////////////
// NOTE: Arena Functions
///////////////////////////////

typedef struct Arena{
    // head
    void* base;
    size_t size;
    size_t used;
} Arena;

// CONSIDER: not sure if I need this, if I have os_alloc_arena()
static Arena* alloc_arena(size_t size){
    void* memory = calloc((size + sizeof(Arena)), 1); // 0 initialized
    Arena* result = (Arena*)memory;
    result->base = (u8*)memory + sizeof(Arena);
    result->size = size;
    result->used = 0;
    return(result);
}

static void arena_init(Arena* arena, void* base, size_t size){
    arena->base = base;
    arena->size = size;
    arena->used = 0;
}

static void arena_free(Arena* arena){
    arena->used = 0;
}

//#define push_size(arena, size) push_size_aligned(arena, size, _Alignof(max_align_t))
#define push_array(arena, type, count) (type*)push_size_aligned((arena), sizeof(type) * (count), _Alignof(type))
#define push_struct(arena, type) (type*)push_size_aligned((arena), sizeof(type), _Alignof(type))
static void* push_size_aligned(Arena* arena, size_t size, size_t align){
    size_t used_aligned = AlignUpPow2(arena->used, align);
    assert((used_aligned + size) <= arena->size);
    void* result = (u8*)arena->base + used_aligned;
    arena->used = used_aligned + size;
    return(result);
}

//static void* push_size_aligned(Arena* arena, size_t size, size_t align){
//    size_t size_aligned = AlignUpPow2(size, align);
//    assert((arena->used + size_aligned) <= arena->size);
//    void* result = (u8*)arena->base + arena->used;
//    arena->used = arena->used + size_aligned;
//    return(result);
//}

//UNTESTED:
#define pop_array(arena, type, count) pop_array_((arena), sizeof(type) * (count))
static void pop_array_(Arena* arena, size_t size){
    arena->used = arena->used - size;
}

static Arena* push_arena(Arena *arena, size_t size){
    Arena* result = push_struct(arena, Arena);
    result->base = push_array(arena, u8, size);
    result->size = size;
    result->used = 0;
    return(result);
}

typedef struct ScratchArena{
    Arena* arena;
    size_t used;
} ScratchArena;

#define DEFAULT_RESERVE_SIZE GB(1)
#define SCRATCH_POOL_COUNT 3
__thread Arena* scratch_pool[SCRATCH_POOL_COUNT] = {};

static ScratchArena get_scratch(Arena* arena){
    ScratchArena result;
    result.arena = arena;
    result.used = arena->used;
    return(result);
}

static ScratchArena
begin_scratch(u32 index){
    // init on first time
    if (scratch_pool[0] == 0){
        Arena **scratch_slot = scratch_pool;
        for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
            Arena* arena = alloc_arena(DEFAULT_RESERVE_SIZE);
            *scratch_slot = arena;
        }
    }
    ScratchArena result = get_scratch((*(scratch_pool + index)));
    return(result);
}

// mostly copy paste, but I understand it
// maybe dont use this, until a find a good use case?
// use begin_scratch() defined above instead for now.
static ScratchArena
_begin_scratch(Arena **conflict_array, u32 count){
    // init on first time
    if (scratch_pool[0] == 0){
        Arena **scratch_slot = scratch_pool;
        for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
            Arena* arena = alloc_arena(DEFAULT_RESERVE_SIZE);
            *scratch_slot = arena;
        }
    }

    // get non-conflicting arena
    ScratchArena result = {};
    Arena **scratch_slot = scratch_pool;
    for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot += 1){
        bool is_non_conflict = true;
        Arena **conflict_ptr = conflict_array;
        for (u32 j = 0; j < count; ++j, conflict_ptr += 1){
            if (*scratch_slot == *conflict_ptr){
                is_non_conflict = false;
                break;
            }
        }
        if (is_non_conflict){
            result = get_scratch(*scratch_slot);
            break;
        }
    }

    return(result);
}

static void end_scratch(ScratchArena scratch){
    scratch.arena->used = scratch.used;
}

//TODO: arena_resize_align

#endif
