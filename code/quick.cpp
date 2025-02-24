#include "base_inc.h"
#include "win32_base_inc.h"


int main(int argc, char** argv){

    char a[] = "11-30-2024";
    char b[] = "11-6-2024";
    s32 result = strcmp(a, b);
    print("%i\n", result);
    result = strcmp(b, a);
    print("%i\n", result);

    return(0);
}
