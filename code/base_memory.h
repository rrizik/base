#if !defined(BASE_MEMORY_H)
#define BASE_MEMORY_H

#include <stdlib.h>
#include "base_types.h"
#include "base_math.h"

///////////////////////////////
// NOTE: Arena Functions
///////////////////////////////

typedef struct Arena{
    void* base;
    size_t size;
    size_t used;
} Arena;

static Arena*
allocate_arena(size_t size){
    void* memory = calloc((size + sizeof(Arena)), 1);
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

#define push_array(arena, type, count) (type*)push_size_aligned(arena, count * sizeof(type), _Alignof(type))
#define push_struct(arena, type) (type*)push_size_aligned(arena, sizeof(type), _Alignof(type))
#define push_size(arena, size) push_size_aligned(arena, size, _Alignof(max_align_t))
static void* push_size_aligned(Arena* arena, size_t size, size_t align){
    size_t used_aligned = AlignUpPow2(arena->used, align);
    Assert(used_aligned + size <= arena->size);
    void* result = (u8*)arena->base + used_aligned;
    arena->used = used_aligned + size;
    return(result);
}

static Arena* push_arena(Arena *arena, size_t size){
    Arena* result = push_struct(arena, Arena);
    result->base = push_size(arena, size);
    result->size = size;
    result->used = 0;
    return(result);
}

typedef struct ScratchArena{
    Arena* arena;
    size_t used;
} ScratchArena;

#define DEFAULT_RESERVE_SIZE GB(1)            
#define SCRATCH_POOL_COUNT 2
__thread Arena* scratch_pool[SCRATCH_POOL_COUNT] = {};

static ScratchArena get_scratch(Arena* arena){
    ScratchArena result;
    result.arena = arena;
    result.used = arena->used;
    return(result);
}

// mostly copy paste, but I understand it
static ScratchArena
begin_scratch(Arena **conflict_array, u32 count){
    // init on first time
    if (scratch_pool[0] == 0){
        Arena **scratch_slot = scratch_pool;
        for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
            Arena* arena = allocate_arena(DEFAULT_RESERVE_SIZE);
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
