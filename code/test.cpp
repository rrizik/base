#include "base_include.h"
#include "win32_base_include.h"

//#define evalb(x)   print("%s = %d\n", #x, (bool)(x));
#define eval(x) ((bool)(x)?print("SUCCEED - %s\n", #x):print("FAILED  - %s\n", #x))
#define evalb(x)   print("%s = %d\n", #x, (bool)(x));
#define evals32(x) print("%s = %d\n", #x, (s32)(x));
#define evals64(x) print("%s = %lld\n", #x, (s64)(x));
#define evalu32(x) print("%s = %u\n", #x, (u32)(x));
#define evalu64(x) print("%s = %llu\n", #x, (u64)(x));
#define evalf32(x) print("%s = %f\n", #x, (f32)(x));
#define evalf64(x) print("%s = %f\n", #x, (f64)(x));


u32 fail_count = 0;
static bool assert(bool x){
    if(!(x)){
        fail_count++;
        return(false);
    }
    return(true);
}


s32 main(s32 argc, char** argv){

    // base_types.h
    {
        //ArrayCount
        s32 array[10];
        eval(assert(ArrayCount(array) == 10));
        print("\n");

        // MIN - do I need to test every type?
        eval(assert(MIN(1, 2) == 1));
        eval(assert(MIN(2, 1) == 1));
        print("\n");

        // MAX
        eval(assert(MAX(1, 2) == 2));
        eval(assert(MAX(2, 1) == 2));
        print("\n");

        // CLAMP
        eval(assert(CLAMP(2,5,4) == 4));
        eval(assert(CLAMP(2,1,4) == 2));
        print("\n");

        // ABS
        eval(assert(ABS(-1) == 1));
        eval(assert(ABS(-1.1) == 1.1));
        print("\n");

        // KB/MB/GB/TB
        eval(assert(KB(1) == 1024LL));
        eval(assert(MB(1) == (1024LL * 1024LL)));
        eval(assert(GB(1) == (1024LL * 1024LL * 1024LL)));
        eval(assert(TB(1) == (1024LL * 1024LL * 1024LL * 1024LL)));
        print("\n");

        // HU/TH/MI/BI/TR
        eval(assert(HUNDRED(1) == 100));
        eval(assert(THOUSAND(1) == 1000));
        eval(assert(MILLION(1) == 1000000));
        eval(assert(BILLION(1) == 1000000000));
        eval(assert(TRILLION(1) == 1000000000000));
        print("\n");

        // type_min_max
        eval(assert(s8_min == -128));
        eval(assert(s16_min == -32768));
        eval(assert(s32_min == -2147483648));
        eval(assert(s64_min == -9223372036854775808ll));
        eval(assert(s8_max == 127));
        eval(assert(s16_max == 32767));
        eval(assert(s32_max == 2147483647));
        eval(assert(s64_max == 9223372036854775807ll));
        eval(assert(u8_max == 255));
        eval(assert(u16_max == 65535));
        eval(assert(u32_max == 4294967295));
        eval(assert(u64_max == 18446744073709551615ull));
        print("\n");

        // compound type operators
        v2s32 _v2s32 = {1, 2};
        eval(assert((_v2s32 + _v2s32) == ((v2s32){2, 4})));
        eval(assert((_v2s32 - _v2s32) == ((v2s32){0, 0})));
        eval(assert((3 * _v2s32) == ((v2s32){3, 6})));
        eval(assert((_v2s32 * 3) == ((v2s32){3, 6})));
        eval(assert(_v2s32 == _v2s32));
        eval(assert(!(_v2s32 != _v2s32)));
        print("\n");
        
        v2 _v2 = {1, 2};
        eval(assert((_v2 + _v2) == ((v2){2, 4})));
        eval(assert((_v2 - _v2) == ((v2){0, 0})));
        eval(assert((3 * _v2) == ((v2){3, 6})));
        eval(assert((_v2 * 3) == ((v2){3, 6})));
        eval(assert(_v2 == _v2));
        eval(assert(!(_v2 != _v2)));
        print("\n");
    
    
        v3 _v3 = {1, 2};
        eval(assert((_v3 + _v3) == ((v3){2, 4})));
        eval(assert((_v3 - _v3) == ((v3){0, 0})));
        eval(assert((3 * _v3) == ((v3){3, 6})));
        eval(assert((_v3 * 3) == ((v3){3, 6})));
        eval(assert(_v3 == _v3));
        eval(assert(!(_v3 != _v3)));
        print("\n");
    
   
        v4 _v4 = {1, 2};
        eval(assert((_v4 + _v4) == ((v4){2, 4})));
        eval(assert((_v4 - _v4) == ((v4){0, 0})));
        eval(assert((3 * _v4) == ((v4){3, 6})));
        eval(assert((_v4 * 3) == ((v4){3, 6})));
        eval(assert(_v4 == _v4));
        eval(assert(!(_v4 != _v4)));
        print("\n");
    }

    // base_math.h
    {
        eval(assert(AlignUpPow2(19, 4) == 20));
        eval(assert(AlignDownPow2(19, 4) == 16));
        print("\n");

        // trig
        eval(assert(round_f32_s32(RAD2DEG(RAD)) == 1));
        eval(assert(DEG2RAD(1) == 0.017453f));
        eval(assert(sqrt_f32(16.0f) == 4.0f));
        eval(assert(round_f32_s32(sin_f32((PI/2))) == 1));
        eval(assert(round_f32_s32(cos_f32((PI*2))) == 1));
        eval(assert(round_f32_s32(tan_f32(PI)) == 0));
        eval(assert(sqrt_f64(16.0) == 4.0));
        eval(assert(round_f64_s64(sin_f64((f64)(PI/2))) == 1));
        eval(assert(round_f64_s64(cos_f64((f64)(PI*2))) == 1));
        eval(assert(round_f64_s64(tan_f64(PI)) == 0));
//static f32 atan_f32(f32 x, f32 y){ return(atan2f(x, y)); }
//static f64 atan_f64(f64 x, f64 y){ return(atan2(x, y)); }
        eval(assert(rad_to_dir(0) == ((v2){1, 0})));
        eval(assert(dir_to_rad((v2){1, 0}) == 0));
        print("\n");

        // abs/round/clamp/trunc/floor
        eval(assert(abs_f32(-1.0123f) == 1.0123f));
        eval(assert(abs_f64(-1.0123) == 1.0123));
        eval(assert(abs_s32(-1) == 1));
        eval(assert(abs_s64(-1) == 1));
        eval(assert(round_v2(((v2){1.1f, 1.9f})) == ((v2){1, 2})));
        eval(assert(round_f32(1.1f) == 1.0f));
        eval(assert(round_f32(1.9f) == 2.0f));
        eval(assert(round_f32_s32(1.1f) == 1));
        eval(assert(round_f32_s32(1.9f) == 2));
        eval(assert(round_f32_u32(1.1f) == 1));
        eval(assert(round_f32_u32(1.9f) == 2));
        eval(assert(trunc_f32(1.9120939f) == 1.0f));
        eval(assert(trunc_f32_s32(1.910293f) == 1));
        eval(assert(floor_f32(1.910293f) == 1.0f));
        eval(assert(floor_f32_s32(1.110293f) == 1));
        eval(assert(clamp_f32(1.0f, 3.0f, 2.0f) == 2.0f));
        eval(assert(clamp_f32(1.0f, 0.5f, 2.0f) == 1.0f));
        eval(assert(clamp_f32(1.0f, 2.0f, 3.0f) == 2.0f));
        print("\n");
    
        eval(assert(lerp(0.0f, 0.5f, 1.0f) == 0.5f));
        eval(assert(unlerp(0.0f, 0.5f, 1.0f) == 0.5f));
//static f32 lerp_rad(f32 a, f32 t, f32 b){
        print("\n");
    }
    {
        Arena* test_arena1 = allocate_arena(100);
        eval(assert(allocate_arena(10)->size == 10));
        Arena* test_arena2 = {};
//        arena_init(&pm->arena, (u8*)memory->permanent_base + sizeof(PermanentMemory), memory->permanent_size - sizeof(PermanentMemory));

        //arena_init(test_arena2, test_arena1->base 
    }
    print("\nFAILED COUNT: %d", fail_count);
    return(0);
    //end
}
//typedef struct Arena{
//    void* base;
//    size_t size;
//    size_t used;
//} Arena;
//
//static void arena_init(Arena* arena, void* base, size_t size){
//    arena->base = base;
//    arena->size = size;
//    arena->used = 0;
//}
//
//static void arena_free(Arena* arena){
//    arena->used = 0;
//}
//
//#define push_array(arena, type, count) (type*)push_size_aligned(arena, count * sizeof(type), _Alignof(type))
//#define push_struct(arena, type) (type*)push_size_aligned(arena, sizeof(type), _Alignof(type))
//#define push_size(arena, size) push_size_aligned(arena, size, _Alignof(s64))
//static void* push_size_aligned(Arena* arena, size_t size, size_t align){
//    size_t used_aligned = AlignUpPow2(arena->used, align);
//    Assert(used_aligned + size <= arena->size);
//    void* result = (u8*)arena->base + used_aligned;
//    arena->used = used_aligned + size;
//    return(result);
//}
//
//static Arena* push_arena(Arena *arena, size_t size){
//    Arena* result = push_struct(arena, Arena);
//    result->base = push_size(arena, size);
//    result->size = size;
//    result->used = 0;
//    return(result);
//}
//
//typedef struct ScratchArena{
//    Arena* arena;
//    size_t used;
//} ScratchArena;
//
//#define DEFAULT_RESERVE_SIZE GB(1)            
//#define SCRATCH_POOL_COUNT 2
//__thread Arena* scratch_pool[SCRATCH_POOL_COUNT] = {};
//
//static ScratchArena get_scratch(Arena* arena){
//    ScratchArena result;
//    result.arena = arena;
//    result.used = arena->used;
//    return(result);
//}
//
//// mostly copy paste, but I understand it
//static ScratchArena
//begin_scratch(Arena **conflict_array, u32 count){
//    // init on first time
//    if (scratch_pool[0] == 0){
//        Arena **scratch_slot = scratch_pool;
//        for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot +=1){
//            Arena* arena = allocate_arena(DEFAULT_RESERVE_SIZE);
//            *scratch_slot = arena;
//        }
//    }
//    
//    // get non-conflicting arena
//    ScratchArena result = {};
//    Arena **scratch_slot = scratch_pool;
//    for (u64 i=0; i < SCRATCH_POOL_COUNT; ++i, scratch_slot += 1){
//        bool is_non_conflict = true;
//        Arena **conflict_ptr = conflict_array;
//        for (u32 j = 0; j < count; ++j, conflict_ptr += 1){
//            if (*scratch_slot == *conflict_ptr){
//                is_non_conflict = false;
//                break;
//            }
//        }
//        if (is_non_conflict){
//            result = get_scratch(*scratch_slot);
//            break;
//        }
//    }
//    
//    return(result);
//}
//
//static void end_scratch(ScratchArena scratch){
//    scratch.arena->used = scratch.used;
//}
//
////TODO: arena_resize_align
//
//#endif

