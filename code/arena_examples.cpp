#include <stdint.h>

#pragma warning(push, 0)
#include <cstddef>
#pragma warning(pop)

#include <windows.h>

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

#include <stdio.h>
static void print(const char* format, ...) {
    char buffer[4096] = {};

    va_list args;
    va_start(args, format);
    s32 result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("%s", buffer);
    OutputDebugStringA(buffer);
}

typedef struct Arena{
    void* base;
    u64 size;
    u64 at;
} Arena;

static Arena* make_arena(u64 size){
    void* memory = calloc((size + sizeof(Arena)), 1); // 0 initialized
    Arena* result = (Arena*)memory;
    result->base = (u8*)memory + sizeof(Arena);
    result->size = size;
    result->at = 0;
    return(result);
}

#define DEFAULT_COMMIT_SIZE (1024 * 4) // 1 page as an example
static Arena* arena_init_reserve(Arena* arena, u64 size){
    arena->base = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
    arena->size = 0;
    arena->at = 0;
    return(arena);
}

#if defined(__SANITIZE_ADDRESS__)
#  define ENABLE_ASAN
#elif defined(__has_feature)
#  if __has_feature(address_sanitizer)
#    #define ENABLE_ASAN
#  endif
#endif

#if defined(ENABLE_ASAN)
   extern "C" void __asan_poison_memory_region(void const volatile *addr, u32 size);
   extern "C" void __asan_unpoison_memory_region(void const volatile *addr, u32 size);
#  define ASAN_POISON_MEMORY_REGION(addr, size) __asan_poison_memory_region((addr), (size))
#  define ASAN_UNPOISON_MEMORY_REGION(addr, size) __asan_unpoison_memory_region((addr), (size))
#else
#  define ASAN_POISON_MEMORY_REGION(addr, size) ((void)(addr), (void)(size))
#  define ASAN_UNPOISON_MEMORY_REGION(addr, size) ((void)(addr), (void)(size))
#endif
static void arena_init(Arena* arena, u64 size){
    arena->base = calloc(size, 1);
    arena->size = size;
    arena->at = 0;
}

static void arena_init_poison(Arena* arena, u64 size){
    arena->base = calloc(size, 1);
    arena->size = size;
    arena->at = 0;
    ASAN_POISON_MEMORY_REGION(arena->base, size);
}

static void arena_free_poison(Arena* arena){
    arena->at = 0;
    ASAN_POISON_MEMORY_REGION(arena->base, arena->size);
}

static void arena_free(Arena* arena){
    arena->at = 0;
}

#define push_array(arena, type, count) (type*)push_size((arena), sizeof(type) * (count))
#define push_struct(arena, type) (type*)push_size((arena), sizeof(type))
static void* push_size(Arena* arena, u64 size){
    if(arena->at + size > arena->size){
        return(0); // or can assert() but this lets you fail gracefully
    }
    void* result = (u8*)arena->base + arena->at;
    arena->at += size;
    return(result);
}

#define AlignUpPow2(x,p) (((x) + (p) - 1)&~((p) - 1))
#define push_array_aligned(arena, type, count) (type*)push_size_aligned((arena), sizeof(type) * (count), alignof(max_align_t))
#define push_struct_aligned(arena, type) (type*)push_size_aligned((arena), sizeof(type), alignof(max_align_t))
static void* push_size_aligned(Arena* arena, u64 size, u64 align){
    u64 at_aligned = AlignUpPow2(arena->at, align);
    if((at_aligned + size) > arena->size){
        return(0); // or can assert() but this lets you fail gracefully
    }
    void* result = (u8*)arena->base + at_aligned;
    arena->at = at_aligned + size;
    return(result);
}

#define push_array_growable(arena, type, count) (type*)push_size_growable((arena), sizeof(type) * (count))
#define push_struct_growable(arena, type) (type*)push_size_growable((arena), sizeof(type))
static void* push_size_growable(Arena* arena, u64 size){
    if(arena->at + size > arena->size){
        u64 commit_size = DEFAULT_COMMIT_SIZE > size ? DEFAULT_COMMIT_SIZE : size;
        VirtualAlloc((u8*)arena->base + arena->at, commit_size, MEM_COMMIT, PAGE_READWRITE);
        arena->size = arena->size + commit_size;
    }
    void* result = (u8*)arena->base + arena->at;
    arena->at += size;
    return(result);
}

#define push_array_unpoison(arena, type, count) (type*)push_size_unpoison((arena), sizeof(type) * (count))
#define push_struct_unpoison(arena, type) (type*)push_size_unpoison((arena), sizeof(type))
static void* push_size_unpoison(Arena* arena, u64 size){
    if(arena->at + size > arena->size){
        return(0);
    }
    void* result = (u8*)arena->base + arena->at;
    ASAN_UNPOISON_MEMORY_REGION((u8*)arena->base + arena->at, size);
    arena->at += size;
    return(result);
}

static void arena_pop_size(Arena* arena, u64 size){
    arena->at = arena->at - size;
}

static void arena_pop_to(Arena* arena, u64 at){
    arena->at = at;
}

// --------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------------------

typedef struct ScratchArena{
    Arena* arena;
    u64 at;
} ScratchArena;

#define SCRATCH_POOL_COUNT 3
static __declspec(thread) Arena* scratch_pool[SCRATCH_POOL_COUNT] = {};

static ScratchArena get_scratch(Arena* arena){
    ScratchArena result;
    result.arena = arena;
    result.at = arena->at;
    return(result);
}

#define DEFAULT_RESERVE_SIZE (1024 * 1024 * 1024) // some size yuo want
static ScratchArena begin_scratch(u32 index){
    // init on first time calling
    if (scratch_pool[0] == 0){
        Arena **scratch_slot = scratch_pool;
        for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
            Arena* arena = make_arena(DEFAULT_RESERVE_SIZE);
            *scratch_slot = arena;
        }
    }
    ScratchArena result = get_scratch((*(scratch_pool + index)));
    return(result);
}

static void end_scratch(ScratchArena scratch){
    scratch.arena->at = scratch.at;
}

typedef struct Test{
    u64 a, b, c, d, e, f, g, h;
} Test;

typedef struct Test_Unaligned{
    u64 a, b, c, d, e, f, g;
    u8 h;
} Test_Unaligned;

s32 main(s32 argc, char** argv){
    u32 a = 100;
    u32 pos = 0;
    while(true){
        u32 v = pos % a;
        pos++;
        print("%i\n", v);
    }
    Arena* arena_a = make_arena(128);
    Test* a1 = push_struct(arena_a, Test);
    if(a1){ print("a1 succeed\n"); }
    else{ print("a1 failed\n"); }

    Test* a2 = push_struct(arena_a, Test);
    if(a2){ print("a2 succeed\n"); }
    else{ print("a2 failed\n"); }

    Arena* arena_b = make_arena(((24 * 3) - 1) + 24);
    u8* b1 = push_array_aligned(arena_b, u8, 3);
    if(b1){ print("b1 succeed\n"); }
    else{ print("b1 failed\n"); }

    u64* b2 = push_array_aligned(arena_b, u64, 6);
    if(b2){ print("b2 succeed\n"); }
    else{ print("b2 failed\n"); }

    u64* b3 = push_array_aligned(arena_b, u64, 3);
    if(b3){ print("b3 succeed\n"); }
    else{ print("b3 failed\n"); }

    Arena arena_c = {0};
    arena_init_reserve(&arena_c, (1024 * 1024 * 1024));
    Test* c;
    for(s32 i=0; i < 1024 * 1024; ++i){
        c = push_struct_growable(&arena_c, Test);
        c->a = 1;
    }


    {
        ScratchArena scratch = begin_scratch(0);
        Test* t1 = push_struct(scratch.arena, Test);
        Test* t2 = push_struct(scratch.arena, Test);
        Test* t3 = push_struct(scratch.arena, Test);
        Test* t4 = push_struct(scratch.arena, Test);
        end_scratch(scratch);
    }

    Arena arena_d = {0};
    arena_init_poison(&arena_d, (1024 * 4));
    u64* d = push_array_unpoison(&arena_d, u64, 100);
    *d = 1;
    return(0);
}

//
//                                                                                        struct Arena{
//                                                                                            void* base;
//                                                                                            u64 size;
//                                                                                            u64 at;
//                                                                                        };
//
//                                                                                        Arena arena;
//                                                                                        arena.base = malloc(very_large_size);
//                                                                                        arena.size = very_large_size;
//                                                                                        arena.at = 0;
//                                                                                        // better if we use calloc() or VirtualAlloc()
//                                                                                        // as its initialized to 0
//
//
//
//                                                                                        #define push_array(arena, type, count) (type*)push_size((arena), sizeof(type) * (count))
//                                                                                        #define push_struct(arena, type) (type*)push_size((arena), sizeof(type))
//                                                                                        void* push_size(Arena* arena, u64 size){...}
//                                                                                        Arena* make_arena(u64 size){...}
//                                                                                        void arena_clear(Arena* arena){..}
//
//                                                                                        // Usage code
//                                                                                        Arena* arena = make_arena(1024 * 4); // 1 page worth of memory
//
//                                                                                        // All valid allocations
//                                                                                        StructA* a = push_struct(arena, StructA);
//                                                                                        StructA* a_array = push_array(arena, StructA, 100);
//                                                                                        char* char_buffer = push_array(arena, char, 1024);
//                                                                                        void* buffer1 = push_size(arena, 1024);
//                                                                                        void* buffer2 = push_size(arena, 1024);
//                                                                                        void* buffer3 = push_size(arena, 1024); // This returns an invalid pointer
//
//                                                                                        if(!buffer3){ print("FAILED ALLOCATION\N"); } // ran out of space
//                                                                                        arena_clear(arena) // now we have all the space available again
//
//
//
//
//
//
//                                                                                            if(arena->at + size > arena->size){
//                                                                                                return(0); // or can assert() but this lets you fail gracefully
//                                                                                            }
//                                                                                            void* result = (u8*)arena->base + arena->at;
//                                                                                            arena->at += size;
//                                                                                            return(result);
//                                                                                        }
//
//                                                                                        void arena_clear(Arena* arena){
//                                                                                            arena->at = 0;
//                                                                                        }
//
//
////
////static Arena* make_arena(u64 size);
////static Arena* arena_init_reserve(Arena* arena, u64 size);
////static void arena_init(Arena* arena, u64 size);
////static void arena_free(Arena* arena);
////
////#define push_array(arena, type, count) (type*)push_size((arena), sizeof(type) * (count))
////#define push_struct(arena, type) (type*)push_size((arena), sizeof(type))
////static void* push_size(Arena* arena, u64 size);
////
////static void arena_pop_size(Arena* arena, u64 size);
////static void arena_pop_to(Arena* arena, u64 at);
////
////// --------------------------------------------------------------------------------------------------------
////// --------------------------------------------------------------------------------------------------------
////
//
//
//
//
//
//
//
//
//
//
//                                                                                    struct ScratchArena{
//                                                                                        Arena* arena;
//                                                                                        u64 at;
//                                                                                    };
//
//                                                                                    ScratchArena get_scratch(Arena* arena){
//                                                                                        ScratchArena result;
//                                                                                        result.arena = arena;
//                                                                                        result.at = arena->at;
//                                                                                        return(result);
//                                                                                    }
//
//                                                                                    void end_scratch(ScratchArena scratch){
//                                                                                        scratch.arena->at = scratch.at;
//                                                                                    }
//
//                                                                                    // Special case where you want a pool of thread local ScratchArenas
//                                                                                    #define SCRATCH_POOL_COUNT 3
//                                                                                    __declspec(thread) Arena* scratch_pool[SCRATCH_POOL_COUNT] = {};
//                                                                                    #define DEFAULT_RESERVE_SIZE (1024 * 1024 * 1024) // some size you want
//                                                                                    ScratchArena begin_scratch(u32 index){
//                                                                                        if (scratch_pool[0] == 0){
//                                                                                            Arena **scratch_slot = scratch_pool;
//                                                                                            for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
//                                                                                                Arena* arena = make_arena(DEFAULT_RESERVE_SIZE);
//                                                                                                *scratch_slot = arena;
//                                                                                            }
//                                                                                        }
//                                                                                        ScratchArena result = get_scratch((*(scratch_pool + index)));
//                                                                                        return(result);
//                                                                                    }
//
//
//
//                                                                                    void d3d_draw_text(Font font, f32 x, f32 y, RGBA color, String8 text){
//                                                                                        ScratchArena scratch = begin_scratch(0); // BEGIN TEMPORARY MEMORY
//
//                                                                                        u64 allocation_size = text.size * 6;
//                                                                                        Vertex* buffer = push_array(scratch.arena, Vertex, allocation_size);
//                                                                                        Vertex* vertex = buffer;
//
//                                                                                        stbtt_aligned_quad quad;
//                                                                                        for(s32 i=0; i < text.size; ++i){
//                                                                                            u8* character = text.str + i;
//                                                                                            stbtt_GetPackedQuad(font.packed_chars, font.texture_w, font.texture_h, (*character) - font.first_char, &x, &y, &quad, 1);
//                                                                                            Rect rect = make_rect(quad.x0, quad.y0, quad.x1, quad.y1);
//                                                                                            Rect clip_rect = rect_pixel_to_clip_inverted(rect, resolution);
//
//                                                                                            *vertex++ = { make_v3(clip_rect.x0, clip_rect.y1, 0.0f), color, make_v2(quad.s0, quad.t1) };
//                                                                                            *vertex++ = { make_v3(clip_rect.x1, clip_rect.y1, 0.0f), color, make_v2(quad.s1, quad.t1) };
//                                                                                            *vertex++ = { make_v3(clip_rect.x0, clip_rect.y0, 0.0f), color, make_v2(quad.s0, quad.t0) };
//                                                                                            *vertex++ = { make_v3(clip_rect.x0, clip_rect.y0, 0.0f), color, make_v2(quad.s0, quad.t0) };
//                                                                                            *vertex++ = { make_v3(clip_rect.x1, clip_rect.y1, 0.0f), color, make_v2(quad.s1, quad.t1) };
//                                                                                            *vertex++ = { make_v3(clip_rect.x1, clip_rect.y0, 0.0f), color, make_v2(quad.s1, quad.t0) };
//                                                                                        }
//                                                                                        ...
//                                                                                        d3d_context->Draw((UINT)allocation_size, 0);
//                                                                                        end_scratch(scratch); // END TEMPORARY MEMORY
//                                                                                    }
//
//
//
//                                                                                    String8 os_get_cwd(Arena* arena){
//                                                                                        ScratchArena scratch = begin_scratch(0); // BEGIN TEMPORARY MEMORY
//
//                                                                                        u32 length = GetCurrentDirectoryW(0, 0);
//                                                                                        wchar* buffer = push_array(scratch.arena, wchar, length);
//
//                                                                                        length = GetCurrentDirectoryW(length, buffer);
//                                                                                        String16 utf16_string = {(u16*)buffer, length};
//
//                                                                                        String8 utf8_string = os_utf16_utf8(arena, utf16_string);
//                                                                                        end_scratch(scratch); // END TEMPORARY MEMORY
//                                                                                        return(utf8_string);
//                                                                                    }
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//                                                                                                    void *p1 = malloc(512);
//                                                                                                    void *p2 = malloc(64);
//                                                                                                    void *p3 = malloc(1024*1024);
//                                                                                                    free(p1);
//                                                                                                    free(p2);
//                                                                                                    free(p3);
//                                                                                                    -----------------------------
//                                                                                                    void *p1 = malloc(512);
//                                                                                                    void *p2 = malloc(64);
//                                                                                                    void *p3 = malloc(1024*1024);
//                                                                                                    free(p3);
//                                                                                                    free(p2);
//                                                                                                    free(p1);
//                                                                                                    -----------------------------
//                                                                                                    void *p1 = malloc(512);
//                                                                                                    void *p2 = malloc(64);
//                                                                                                    void *p3 = malloc(1024*1024);
//                                                                                                    // (no freeing whatsoever)
//                                                                                                    -----------------------------
//                                                                                                    void *p1 = malloc(512);
//                                                                                                    free(p1);
//                                                                                                    void *p2 = malloc(64); // part of p1 used here?
//                                                                                                    void *p3 = malloc(1024*1024);
//                                                                                                    free(p2);
//                                                                                                    free(p3);
//                                                                                                    void *p4 = malloc(7); // part of p1/p2 used here?
//                                                                                                    free(p4);
//
//
//
//
//
//                                                                                                    struct Struct{
//                                                                                                        void* data
//                                                                                                        ...
//                                                                                                        int x;
//                                                                                                    };
//
//                                                                                                    Struct* struct_alloc(void){
//                                                                                                      SomeStruct* result = malloc(sizeof(SomeStruct));
//                                                                                                      // initializations + potentially other mallocs needed for the struct
//                                                                                                      return result;
//                                                                                                    }
//
//                                                                                                    void struct_release(Struct *value){
//                                                                                                      // deinitinitialization +  other frees that are necessary for the struct
//                                                                                                      free(value);
//                                                                                                    }
//
//
//
//
//                                                                                                    struct StructD{
//                                                                                                        // Does its own allocations
//                                                                                                    }
//
//                                                                                                    struct StructC{
//                                                                                                        // Data
//                                                                                                    }
//
//                                                                                                    struct StructB{
//                                                                                                        StructD* d;
//                                                                                                        // Data
//                                                                                                    }
//
//                                                                                                    struct StructA{
//                                                                                                        StructB* b;
//                                                                                                        StructC* c;
//                                                                                                        // Data
//                                                                                                    }
//
//
//
//
//                                                                                                    class MyClass {
//                                                                                                    public:
//                                                                                                        MyClass() {
//                                                                                                            std::cout << "Constructor called" << std::endl;
//                                                                                                        }
//
//                                                                                                        ~MyClass() {
//                                                                                                            std::cout << "Destructor called" << std::endl;
//                                                                                                        }
//
//                                                                                                        void hello() {
//                                                                                                            std::cout << "Hello!" << std::endl;
//                                                                                                        }
//                                                                                                    };
//
//                                                                                                    -----------------------------
//                                                                                                    {
//                                                                                                        MyClass result;
//                                                                                                        result.hello();
//                                                                                                    } // destructor automatically called on scope exit
//                                                                                                    -----------------------------
//                                                                                                    {
//                                                                                                        MyClass* result = new MyClass;
//                                                                                                        result.hello();
//                                                                                                        delete result; // destructor automatically called on delete
//                                                                                                    }
//                                                                                                    -----------------------------
//                                                                                                    {
//                                                                                                        std::unique_ptr<MyClass> result = std::make_unique<MyClass>();
//                                                                                                        result.hello();
//                                                                                                    } // destructor automatically called on scope exit
//                                                                                                    -----------------------------
//
//
//                         --------------------------------------------
//                        |                | Size Known | Size Unknown |
//                         --------------------------------------------
//                        |  Lifetime Known|    95%     |     ~4%      |
//                         --------------------------------------------
//                        |Lifetime Unknown|    ~1%     |     <1%      |
//                         --------------------------------------------
//
//
//
//
//
//
//                                                                        // Growable storage & contiguous address space
//                                                                        Arena* arena_init(Arena* arena, u64 size){
//                                                                            arena->base = VirtualAlloc(0, size, MEM_RESERVE, PAGE_READWRITE);
//                                                                            arena->size = 0;
//                                                                            arena->at = 0;
//                                                                            return(arena);
//                                                                        }
//
//                                                                        void* push_size(Arena* arena, u64 size){
//                                                                            if(arena->at + size > arena->size){
//                                                                                u64 commit_size = DEFAULT_COMMIT_SIZE > size ? DEFAULT_COMMIT_SIZE : size;
//                                                                                VirtualAlloc((u8*)arena->base + arena->at, commit_size, MEM_COMMIT, PAGE_READWRITE);
//                                                                                arena->size = arena->size + commit_size;
//                                                                            }
//                                                                            void* result = (u8*)arena->base + arena->at;
//                                                                            arena->at += size;
//                                                                            return(result);
//                                                                        }
//
//
//
//
//                                                                        // Poison memory regions for additional safety
//                                                                        void arena_init(Arena* arena, u64 size){
//                                                                            arena->base = calloc(size, 1);
//                                                                            arena->size = size;
//                                                                            arena->at = 0;
//                                                                            __asan_poison_memory_region(arena->base, size);
//                                                                        }
//
//                                                                        void* push_size(Arena* arena, u64 size){
//                                                                            if(arena->at + size > arena->size){
//                                                                                return(0);
//                                                                            }
//                                                                            void* result = (u8*)arena->base + arena->at;
//                                                                            __asan_unpoison_memory_region((u8*)arena->base + arena->at, size);
//                                                                            arena->at += size;
//                                                                            return(result);
//                                                                        }
//
//                                                                        void arena_clear(Arena* arena){
//                                                                            arena->at = 0;
//                                                                            __asan_poison_memory_region(arena->base, arena->size);
//                                                                        }
//






