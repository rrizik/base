#pragma once

#include <stdio.h>
static void print(char* format, ...) {
    char buffer[4096] = {};
    va_list args;
    va_start(args, format);
    s32 result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("%s", buffer);
    OutputDebugStringA(buffer);
}

[[ noreturn ]] static void // QUESTION: what is this?
fatal_error(const char* message){
    MessageBoxA(0, message, "Error", MB_ICONEXCLAMATION);
    ExitProcess(0);
}
