#include "base_include.h"
#include "win32_base_include.h"

#define PRINT_SUCCEED 1

#if PRINT_SUCCEED
#  define evall(x) ((bool)(x)?print("SUCCEED - %ls\n", #x):print(" FAILED - %ls\n", #x))
#  define eval(x)  ((bool)(x)?print("SUCCEED - %s\n", #x):print(" FAILED - %s\n", #x))
#else
#  define evall(x) ((bool)(x)?print(""):print(" FAILED - %ls\n", #x))
#  define eval(x)  ((bool)(x)?print(""):print(" FAILED - %s\n", #x))
#endif
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

        // MIN - do I need to test every type?
        eval(assert(MIN(1, 2) == 1));
        eval(assert(MIN(2, 1) == 1));

        // MAX
        eval(assert(MAX(1, 2) == 2));
        eval(assert(MAX(2, 1) == 2));

        // CLAMP
        eval(assert(CLAMP(2,5,4) == 4));
        eval(assert(CLAMP(2,1,4) == 2));

        // ABS
        eval(assert(ABS(-1) == 1));
        eval(assert(ABS(-1.1) == 1.1));

        // KB/MB/GB/TB
        eval(assert(KB(1) == 1024LL));
        eval(assert(MB(1) == (1024LL * 1024LL)));
        eval(assert(GB(1) == (1024LL * 1024LL * 1024LL)));
        eval(assert(TB(1) == (1024LL * 1024LL * 1024LL * 1024LL)));

        // HU/TH/MI/BI/TR
        eval(assert(HUNDRED(1) == 100));
        eval(assert(THOUSAND(1) == 1000));
        eval(assert(MILLION(1) == 1000000));
        eval(assert(BILLION(1) == 1000000000));
        eval(assert(TRILLION(1) == 1000000000000));

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

        // compound type operators
        v2s32 _v2s32 = {1, 2};
        eval(assert((_v2s32 + _v2s32) == ((v2s32){2, 4})));
        eval(assert((_v2s32 - _v2s32) == ((v2s32){0, 0})));
        eval(assert((3 * _v2s32) == ((v2s32){3, 6})));
        eval(assert((_v2s32 * 3) == ((v2s32){3, 6})));
        eval(assert(_v2s32 == _v2s32));
        eval(assert(!(_v2s32 != _v2s32)));
        
        v2 _v2 = {1, 2};
        eval(assert((_v2 + _v2) == ((v2){2, 4})));
        eval(assert((_v2 - _v2) == ((v2){0, 0})));
        eval(assert((3 * _v2) == ((v2){3, 6})));
        eval(assert((_v2 * 3) == ((v2){3, 6})));
        eval(assert(_v2 == _v2));
        eval(assert(!(_v2 != _v2)));
    
    
        v3 _v3 = {1, 2};
        eval(assert((_v3 + _v3) == ((v3){2, 4})));
        eval(assert((_v3 - _v3) == ((v3){0, 0})));
        eval(assert((3 * _v3) == ((v3){3, 6})));
        eval(assert((_v3 * 3) == ((v3){3, 6})));
        eval(assert(_v3 == _v3));
        eval(assert(!(_v3 != _v3)));
    
   
        v4 _v4 = {1, 2};
        eval(assert((_v4 + _v4) == ((v4){2, 4})));
        eval(assert((_v4 - _v4) == ((v4){0, 0})));
        eval(assert((3 * _v4) == ((v4){3, 6})));
        eval(assert((_v4 * 3) == ((v4){3, 6})));
        eval(assert(_v4 == _v4));
        eval(assert(!(_v4 != _v4)));
    }

    // base_math.h
    {
        eval(assert(AlignUpPow2(19, 4) == 20));
        eval(assert(AlignDownPow2(19, 4) == 16));

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
    
        eval(assert(lerp(0.0f, 0.5f, 1.0f) == 0.5f));
        eval(assert(unlerp(0.0f, 0.5f, 1.0f) == 0.5f));
//static f32 lerp_rad(f32 a, f32 t, f32 b){ 
    }
    {
        // TEST ARENA LATER
        Arena* test_arena1 = allocate_arena(100);
        eval(assert(allocate_arena(10)->size == 10));
        Arena* test_arena2 = {};
//        arena_init(&pm->arena, (u8*)memory->permanent_base + sizeof(PermanentMemory), memory->permanent_size - sizeof(PermanentMemory));

        //arena_init(test_arena2, test_arena1->base 
    }
    {
        typedef struct Data{
            u32 id;
        } Data;

                //Node* node = push_node(tm->LL_arena);
                //node->data = e;
                //dll_push_front(&pm->ants, node);


        Arena* arena = allocate_arena(MB(1));
        Node* sentinel = push_node(arena);
        eval(assert(sentinel->next == sentinel));
        eval(assert(sentinel->prev == sentinel));

        Node* n0 = push_node(arena);
        Node* n1 = push_node(arena);
        Node* n2 = push_node(arena);
        Node* n3 = push_node(arena);
        Node* n4 = push_node(arena);

        n0->data = (Data*)push_struct(arena, Data);
        n1->data = (Data*)push_struct(arena, Data);
        n2->data = (Data*)push_struct(arena, Data);
        dll_push_front(sentinel, n0);
        dll_push_back(sentinel, n1);
        dll_push_back(sentinel, n2);
        eval(assert(sentinel->next == n0));
        eval(assert(sentinel->prev == n2));
        eval(assert(sentinel->prev->prev == n1));
        dll_pop_front(sentinel);
        eval(assert(sentinel->next == n1));
        dll_pop_back(sentinel);
        eval(assert(sentinel->prev == n1));
        dll_remove(sentinel, n1);
        eval(assert(sentinel->next == sentinel));
        eval(assert(sentinel->prev == sentinel));
        dll_push_front(sentinel, n0);
        dll_push_back(sentinel, n1);
        dll_push_back(sentinel, n2);
        reset_sentinel(sentinel);
        eval(assert(sentinel->next == sentinel));
        eval(assert(sentinel->prev == sentinel));
    }
    {
        eval(assert(str8_literal("haha").length == 4));
        eval(assert(*str8_literal("haha").str == 'h'));
        eval(assert(str8("haha", 4).length == 4));
        eval(assert(*str8("haha", 4).str == 'h'));

        eval(assert(str16("haha", 4).length == 4));
        eval(assert(*str16("haha", 4).str == L'h'));
        eval(assert(str32("haha", 4).length == 4));
        eval(assert(*str32("haha", 4).str == L'h'));

        ScratchArena scratch = begin_scratch(0, 0);
        String8 left = str8_literal("Hello ");
        String8 middle = str8_literal("World");
        String8 right = str8_literal("!");
        String8 left_middle = str8_concatenate(scratch.arena, left, middle);
        eval(assert((left.length + middle.length) == 11));
        eval(assert(*(left_middle.str + 10) == 'd'));
        String8 full = str8_concatenate(scratch.arena, left_middle, right);
        eval(assert((left.length + middle.length + right.length) == 12));
        eval(assert(*(full.str + 11) == '!'));
    }
    print("\nFAILED COUNT: %d", fail_count);
    return(0);
    //end
}
