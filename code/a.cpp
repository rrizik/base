#include "base_inc.h"
#include "win32_base_inc.h"


s32 main(s32 argc, char** argv){
    m4 m4_1 = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };
    m4 m4_2 = {
        1, 1, 1, 1,
        2, 2, 2, 2,
        3, 3, 3, 3,
        4, 4, 4, 4
    };
    m4 r = m4_1 * m4_2;

    v3 forward = {0, 0, 1};
    v3 up = {0, 1, 0};
    v3 right = normalized_v3(cross_product_v3(forward, up));

    return(0);
}

