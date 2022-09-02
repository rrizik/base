#include "base_inc.h"
#include "win32_base_inc.h"


s32 main(){
    v2 a = {3, 3};
    v2 b = {1, 0};
    v2 c = reflection_v2(a, b);

    return(0);
}

