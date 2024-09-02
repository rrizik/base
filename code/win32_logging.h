#ifndef WIN32_LOGGING
#define WIN32_LOGGING

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

#define print_last_error(error) print_last_error_(error, __LINE__, str8_literal(__FILE__))
static void print_last_error_(DWORD error, s32 line_number, String8 path){
    LPCWSTR message = NULL;

    u32 size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ARGUMENT_ARRAY | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, error, 0, (LPWSTR) &message, 0, NULL);

    String8 file = str8_path_file(path);
    print("%s:%i error:(%i) %ls", file.str, line_number, error, message);
}

#endif
