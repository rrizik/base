#if !defined(WIN32_MEMORY_H)
#define WIN32_MEMORY_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "base_types.h"
#include "base_memory.h"
#include "base_string.h"

///////////////////////////////
// NOTE: Win32 Memory
///////////////////////////////

// TODO(rr): make a os_reserve_alloc() os_commit_alloc() to make a distinction between the two operations
static void* os_virtual_alloc(u64 size){
    // NOTE: VirtualAlloc() && calloc() initializes its allocation to 0, malloc() does not
    void* result = VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    return(result);
}

static bool os_virtual_free(void* base){
    bool result = false;
    if(base){
        result = VirtualFree(base, 0, MEM_RELEASE);
    }
    return(result);
}

static Arena* os_make_arena(u32 size){
    void* memory = os_virtual_alloc((size + sizeof(Arena)));
    Arena* result = (Arena*)memory;
    result->base = (u8*)memory + sizeof(Arena);
    result->size = size;
    result->at = 0;
    return(result);
}

#endif
