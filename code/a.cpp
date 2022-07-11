
#include "win32_base_inc.h"


DWORD thread_proc(LPVOID param){
    char* string = (char*)param;
    for(;;){
        OutputDebugString(string);
        Sleep(1000);
    }
}

typedef struct OK{
    u32 size = 10;
    u32 next = 0;
}OK;

s32 main(s32 argc, char** argv){
    //char* param = "Thread started!\n";
    //DWORD thread_id;
    //HANDLE thread_handle = CreateThread(0, 0, thread_proc, param, 0, &thread_id);
    //CloseHandle(thread_handle);
    
    u32 count = 0;


    OK ok;

    while(count != 3){
        print("%i\n", ok.next);
        ok.next = (ok.next + 1) % ok.size;
        if(ok.next == 9){
            count++;
        }
    }



}

