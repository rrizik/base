#include "base_inc.h"
#include "win32_base_inc.h"

enum Flags{
    FLAG_A,
    FLAG_B,
    FLAG_C
};
static u32 flags;


s32 main(s32 argc, char** argv){
    flags = FLAG_A | FLAG_B;

    return(0);
}

