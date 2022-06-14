#if !defined(WIN32_FILE_H)
#define WIN32_FILE_H

#include <windows.h>
#include "base_types.h"
#include "base_memory.h"
#include "base_string.h"

#include <stdio.h>
static void
print(char *format, ...) {
    char buffer[4096] = {};
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("%s", buffer);
    OutputDebugStringA(buffer);
}

///////////////////////////////
// NOTE: Win32 File Paths
///////////////////////////////

// UNTESTED: test this with a malloc so its not zero'ed out
// and you can tell that you set a null terminator
// TODO: need none OS dependant version of this in string file
static String8
os_utf16_to_utf8(Arena* arena, String16 utf16_string){
    u8* str = (u8*)push_size(arena, utf16_string.length + 1);
    String8 result = {str, utf16_string.length};

    u32 size =  WideCharToMultiByte(CP_UTF8, 0, (wchar*)utf16_string.str, utf16_string.length, (char*)result.str, result.length, 0, 0);

    u8* null_char = result.str + size;
    *null_char = 0;

    return(result);
}

// UNTESTED: test this with a malloc so its not zero'ed out 
// and you can tell that you set a null terminator
// TODO: need none OS dependant version of this in string file
static String16
os_utf8_to_utf16(Arena* arena, String8 utf8_string){
    u16* str = (u16*)push_size(arena, utf8_string.length + 1);
    String16 result = {str, utf8_string.length};

    u32 size = MultiByteToWideChar(CP_UTF8, 0, (char*)utf8_string.str, utf8_string.length, (wchar*)result.str, result.length);

    u16* null_char = result.str + size;
    *null_char = 0;

    return(result);
}

static String8 os_get_cwd(Arena* arena){
    String8 result;

    ScratchArena scratch = begin_scratch(0, 0);
    u32 length = GetCurrentDirectoryW(0, 0);
    wchar* buffer = push_array(scratch.arena, wchar, length);

    length = GetCurrentDirectoryW(length, buffer);
    String16 utf16_string = {(u16*)buffer, length};

    result = os_utf16_to_utf8(arena, utf16_string);
    end_scratch(scratch);
    return(result);
}

// path to binary/.exe
// path for user config/data
// path where temporary files can go

///////////////////////////////
// NOTE: Win32 File I/O
///////////////////////////////

typedef struct FileData{
    void* base;
    u32 size;
} FileData;

static FileData
os_file_read(Arena* arena, String8 dir, String8 file_name){
    FileData result = {0};
    ScratchArena scratch = begin_scratch(0, 0);
    String8 full_path = str8_concatenate(scratch.arena, dir, file_name);
    String16 wide_path = os_utf8_to_utf16(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if(file_handle != INVALID_HANDLE_VALUE){
        LARGE_INTEGER LARGE_file_size;
        if(GetFileSizeEx(file_handle, &LARGE_file_size)){
            u32 file_size = (u32)LARGE_file_size.QuadPart;

            result.base = push_size(arena, file_size);
            if(result.base){
                DWORD bytes_read;
                if(ReadFile(file_handle, result.base, file_size, &bytes_read, 0)){
                    if(bytes_read == file_size){
                        result.size = file_size;
                    }
                    else{
                        print("os_file_read: bytes_read != file_size\n");
                    }
                }
                else{
                    print("os_file_read: failed to read file\n");
                }
            }
            else{
                print("os_file_read: failed to allocate memory\n");
            }
        }
        else{
            print("os_file_read: failed to get file size\n");
        }
        CloseHandle(file_handle);
    }
    else{
        print("os_file_read: failed to create file handle\n");
    }
    return(result);
}

static bool
os_file_write(FileData file, String8 dir, String8 file_name){
    bool result = false;
    ScratchArena scratch = begin_scratch(0, 0);
    String8 full_path = str8_concatenate(scratch.arena, dir, file_name);
    String16 wide_path = os_utf8_to_utf16(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if(file_handle != INVALID_HANDLE_VALUE){
        DWORD bytes_written;
        if(WriteFile(file_handle, file.base, file.size, &bytes_written, 0)){
            result = (file.size == bytes_written);
        }
        CloseHandle(file_handle);
    }
    else{
        print("os_file_write: failed to create file handle\n");
    }
    end_scratch(scratch);
    return(result);
}

///////////////////////////////
// NOTE: Win32 File Operations
///////////////////////////////

static bool
os_file_delete(String8 dir, String8 file_name){
    ScratchArena scratch = begin_scratch(0, 0);
    String8 full_path = str8_concatenate(scratch.arena, dir, file_name);
    String16 wide_path = os_utf8_to_utf16(scratch.arena, full_path);
    bool result = DeleteFileW((wchar*)wide_path.str);
    end_scratch(scratch);
    return(result);
}

static bool
os_file_move(String8 source_file_path, String8 dest_file_path){
    ScratchArena scratch = begin_scratch(0, 0);
    String16 wide_source = os_utf8_to_utf16(scratch.arena, source_file_path);
    String16 wide_dest = os_utf8_to_utf16(scratch.arena, dest_file_path);
    bool result = MoveFileW((wchar*)wide_source.str, (wchar*)wide_dest.str);
    end_scratch(scratch);
    return(result);
}

static bool
os_directory_create(String8 dir_path){
    ScratchArena scratch = begin_scratch(0, 0);
    String16 wide_path = os_utf8_to_utf16(scratch.arena, dir_path);
    bool result = CreateDirectoryW((wchar*)wide_path.str, 0);
    end_scratch(scratch);
    return(result);
}

static bool
os_directory_delete(String8 dir_path){
    ScratchArena scratch = begin_scratch(0, 0);
    String16 wide_path = os_utf8_to_utf16(scratch.arena, dir_path);
    bool result = RemoveDirectoryW((wchar*)wide_path.str);
    end_scratch(scratch);
    return(result);
}

// list out the files in a directory
#endif

