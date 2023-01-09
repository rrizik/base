#if !defined(WIN32_MEMORY_H)
#define WIN32_MEMORY_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "base_types.h"
#include "base_memory.h"
#include "base_string.h"

///////////////////////////////
// NOTE: Win32 Memory
///////////////////////////////

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

static Arena* os_alloc_arena(size_t size){
    void* memory = os_virtual_alloc((size + sizeof(Arena)));
    Arena* result = (Arena*)memory;
    result->base = (u8*)memory + sizeof(Arena);
    result->size = size;
    result->used = 0;
    return(result);
}

#endif

