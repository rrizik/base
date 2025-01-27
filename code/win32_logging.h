#ifndef WIN32_LOGGING
#define WIN32_LOGGING

#include <Windows.h>
#include "base_types.h"
#include "base_memory.h"
#include "base_string.h"

#pragma comment(lib, "user32")

static void print(const char* format, ...) {
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

#define print_last_error(error) print_last_error_(error, __LINE__, str8_literal(__FILE__), str8_literal(__FUNCTION__))
static void print_last_error_(DWORD error, s32 line_number, String8 path, String8 func){
    LPCWSTR message = NULL;

    u32 size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS |
                              FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                              NULL, error, 0, (LPWSTR) &message, 0, NULL);

    //String8 file = str8_path_file(path);
    //char buffer[1024];
    //wchar wbuffer[1024];

    //snprintf(buffer, sizeof(buffer), "%s[%i] error %i: %s(): %ls", file.str, line_number, (s32)error, func.str, message);
    //MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wbuffer, 1024);

    //MessageBoxW(0, wbuffer, 0, MB_OK);

    String8 file = str8_path_file(path);
    print("Error %i: %s[%i] %s(): %ls", (s32)error, file.str, line_number, func.str, message);
}

// todo(rr): implement this
static void message_box_last_error();

#define error_and_quit(message) error_and_quit_(message, __LINE__, str8_literal(__FILE__), str8_literal(__FUNCTION__))
static void error_and_quit_(String8 message, s32 line_number, String8 path, String8 func){
    String8 file = str8_path_file(path);
    char buffer[1024];
    wchar wbuffer[1024];

    snprintf(buffer, sizeof(buffer), "%s[%s(), %i]: error: %s", file.str, func.str, line_number, message.str);
    MultiByteToWideChar(CP_UTF8, 0, buffer, -1, wbuffer, 1024);

    MessageBoxW(0, wbuffer, 0, MB_OK);
    ExitProcess(EXIT_FAILURE);
}

#endif
