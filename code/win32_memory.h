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

static void*
os_alloc(u64 size){
    // NOTE: VirtualAlloc() && calloc() initializes its allocation to 0, malloc() does not
    void* result = VirtualAlloc(0, size, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
    return(result);
}

static void*
os_reserve(u64 size){
    void* base = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
    return(base);
}

static bool
os_commit(void* base, u64 size){
    bool result = (VirtualAlloc(base, size, MEM_COMMIT, PAGE_READWRITE) != 0);
    return(result);
}


static bool
os_free(void* base){
    bool result = false;
    if(base){
        result = (VirtualFree(base, 0, MEM_RELEASE) != 0);
    }
    return(result);
}

static Arena* os_make_arena(u32 size){
    void* memory = os_alloc((size + sizeof(Arena)));
    Arena* result = (Arena*)memory;
    result->base = (u8*)memory + sizeof(Arena);
    result->size = size;
    result->at = 0;
    return(result);
}

#endif
