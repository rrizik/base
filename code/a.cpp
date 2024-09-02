#include "base_inc.h"
#include "win32_base_inc.h"


s32 main(s32 argc, char** argv){

    u8* b;
    Arena* arena = make_arena(KB(1));
    for(s32 i=0; i < 80; ++i){
        b = push_array(arena, u8, 8);
        b[0] = 1;
        b[1] = 1;
        b[2] = 1;
        b[3] = 1;
        b[4] = 1;
        b[5] = 1;
        b[6] = 1;
        b[7] = 1;
    }

    arena_free(arena);
    b = push_array(arena, u8, 8);
    b = push_array_zero(arena, u8, 8);
    b = push_array_zero(arena, u8, 8);
    b = push_array_zero(arena, u8, 8);
        b[0] = 1;
        b[1] = 1;
        b[2] = 1;
        b[3] = 1;
        b[4] = 1;
        b[5] = 1;
        b[6] = 1;
        b[7] = 1;
    b = push_array_zero(arena, u8, 8);
    b = push_array_zero(arena, u8, 8);

    b = push_struct(arena, u8);
    b = push_struct(arena, u8);
    b = push_struct(arena, u8);
    b = push_struct(arena, u8);
    b = push_struct(arena, u8);
    b = push_struct(arena, u8);
    b = push_struct(arena, u8);
    b = push_struct(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);

    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);

        b[0] = 1;
        b[1] = 1;
        b[2] = 1;
        b[3] = 1;
        b[4] = 1;
        b[5] = 1;
        b[6] = 1;
        b[7] = 1;

    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    b = push_struct_zero(arena, u8);
    return(0);
}
