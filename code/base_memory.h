#ifndef BASE_MEMORY_H
#define BASE_MEMORY_H

#include "base_types.h"
#include "base_math.h"

// NOTE: memory_set() and memory_copy() should only be used when we get off of CRT.
//       Otherwise use memset() memcpy()
static void*
memory_set(void* _base, int value, u64 size) {

  u8 *base = (u8 *)_base;
  u64 spread = 0x0101010101010101ULL * value;

  while (size >= 32) {
    ((u64 *)base)[0] = spread;
    ((u64 *)base)[1] = spread;
    ((u64 *)base)[2] = spread;
    ((u64 *)base)[3] = spread;
    base += 32;
    size -= 32;
  }
  while (size >= 16) {
    ((u64 *)base)[0] = spread;
    ((u64 *)base)[1] = spread;
    base += 16;
    size -= 16;
  }

  while (size >= 8) {
    *(u64 *)base = spread;
    base += 8;
    size -= 8;
  }
  while (size >= 4) {
    *(u32 *)base = (u32)spread;
    base += 4;
    size -= 4;
  }
  while (size >= 2) {
    *(u16 *)base = (u16)spread;
    base += 2;
    size -= 2;
  }
  while (size >= 1) {
    *(u8 *)base = (u8)spread;
    base += 1;
    size -= 1;
  }

    return(base);
}

// todo: this needs to be optimized in a similar way to memory_set
static void
memory_copy(void *dst, void *src, u64 size) {
    u8* d = (u8*)dst;
    u8* s = (u8*)src;
    while(size--) {
        *d++ = *s++;
    }
}

///////////////////////////////
// NOTE: Arena
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
    arena->at = 0;
}

static void arena_free_zero(Arena* arena){
    memset(arena->base, 0, arena->at);
    arena->at = 0;
}

// todo: push_array/push_struct should default to zeroing I think
#define push_array(arena, type, count) (type*)push_size_aligned((arena), (u32)(sizeof(type) * (u32)(count)), alignof(type))
#define push_array_zero(arena, type, count) (type*)(memset(push_size_aligned((arena), (u32)(sizeof(type) * (u32)(count)), alignof(type)), 0, (u32)(sizeof(type) * (u32)(count))))
#define push_struct(arena, type) (type*)push_size_aligned((arena), sizeof(type), alignof(type))
#define push_struct_zero(arena, type) (type*)(memset(push_size_aligned((arena), sizeof(type), alignof(type)), 0, sizeof(type)))
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

///////////////////////////////
// NOTE: Scratch Arena
///////////////////////////////

typedef struct ScratchArena{
    Arena* arena;
    u32 at;
    u32 idx;
} ScratchArena;

// todo: make it so that this is a growable arena where I reserve the size and commit what I need.
#define DEFAULT_RESERVE_SIZE GB(1)
#define SCRATCH_POOL_COUNT 3
global THREAD_LOCAL Arena* scratch_pool[SCRATCH_POOL_COUNT] = {};
static u32 scratch_index = 0;

// todo: change to make_scratch()
static ScratchArena get_scratch(Arena* arena){
    ScratchArena result;
    result.arena = arena;
    result.at = arena->at;
    return(result);
}

s32 begin_scratch_count = 0;
s32 end_scratch_count = 0;
static ScratchArena
begin_scratch(Arena* arena=0){
    begin_scratch_count++;
    // note: init scratch memory on first call
    static s32 index = 0;
    if (scratch_pool[0] == 0){
        Arena **scratch_slot = scratch_pool;
        for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
            Arena* new_arena = make_arena(DEFAULT_RESERVE_SIZE);
            *scratch_slot = new_arena;
        }
    }

    //ScratchArena scratch = {0};
    //for(s32 i=0; i < 2; i++){
    //    Arena* pool_arena = *scratch_pool + i;
    //    if(arena != pool_arena){
    //        scratch = get_scratch(arena);
    //        break;
    //    }

    //}
    //return(scratch);

    // note: choose ABCABC scratch arena
    ScratchArena result = get_scratch((*(scratch_pool + (scratch_index % SCRATCH_POOL_COUNT))));
    result.idx = scratch_index;
    scratch_index++;
    //if(arena){
    //    if(arena == result.arena){
    //        result = get_scratch((*(scratch_pool + (scratch_index++ % SCRATCH_POOL_COUNT))));
    //    }
    //}
    return(result);
}

static void end_scratch(ScratchArena scratch){
    end_scratch_count++;
    scratch_index--;
    memset((u8*)scratch.arena->base + scratch.at, 0, scratch.arena->at - scratch.at);
    scratch.arena->at = scratch.at;
}

///////////////////////////////
// NOTE: Pool Arena
///////////////////////////////

typedef struct PoolFreeNode{
    PoolFreeNode* next;
} PoolFreeNode;

typedef struct PoolArena{
    void* base;
    u32 size;
    u32 chunk_size;
    s32 chunk_total;
    s32 chunk_at;

    PoolFreeNode* head;
} PoolArena;

static PoolArena*
make_pool(u32 chunk_size, u32 count){
    void* memory = calloc((chunk_size * count) + sizeof(PoolArena), 1);
    PoolArena* result = (PoolArena*)memory;
    result->base = (u8*)memory + sizeof(PoolArena);
    result->size = chunk_size * count;
    result->chunk_size = chunk_size;
    return(result);
}

static PoolArena*
push_pool(Arena* arena, u32 chunk_size, u32 count){
    PoolArena* result = push_struct(arena, PoolArena);
    result->base = push_array(arena, u8, (chunk_size * count));
    result->size = chunk_size * count;
    result->chunk_size = chunk_size;
    result->chunk_total = (s32)(result->size/result->chunk_size);
    result->chunk_at  = 0;
    return(result);
}

static void
pool_free_all(PoolArena* p){
    memset(p->base, 0, p->size);
    s32 chunk_count = (s32)(p->size/p->chunk_size);

    for(s32 i=0; i < chunk_count; ++i){
        PoolFreeNode* node = (PoolFreeNode*)((u8*)p->base + (i * p->chunk_size));

        node->next = p->head;
        p->head = node;
    }
    p->chunk_at  = 0;
}

static void
pool_free(PoolArena* p, void* ptr){
    if(ptr == 0){
        assert(0);
    }

    void* start = p->base;
    void* end = (u8*)p->base + p->size;
    if(!(start <= (void*)ptr && (void*)ptr < end)){
        assert(0);
    }

    memset(ptr, 0, p->chunk_size);

    PoolFreeNode* node = (PoolFreeNode*)ptr;
    node->next = p->head;
    p->head = node;
    --p->chunk_at;
}

static void*
pool_next(PoolArena* p){
    PoolFreeNode* node = p->head;

    assert(node != 0);

    p->head = p->head->next;
    memset(node, 0, p->chunk_size);
    ++p->chunk_at;
    return(node);
}

#endif
