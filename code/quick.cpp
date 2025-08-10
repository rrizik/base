#include "base_inc.h"
#include "win32_base_inc.h"


int main(int argc, char** argv){

    Arena* arena = make_arena(1024);
    String8 a = {0};
    a.str = push_array(arena, u8, 10);
    //a = str8("123456", 6);
    a = str8_lit("123456");
    String8 b = str8(a.data, a.count);


    //String8 a; = str8_literal("123456");

    print("%s\n", a.str);
    arena_free_zero(arena);
    print("%s\n", b.str);

    return(0);
}
