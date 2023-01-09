#if !defined(WIN32_FILE_H)
#define WIN32_FILE_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "base_types.h"
#include "base_memory.h"
#include "base_string.h"

// TODO: This probably needs to be part if an print logging file
#include <stdio.h>
static void
print(char* format, ...) {
    char buffer[4096] = {};
    va_list args;
    va_start(args, format);
    s32 result = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    printf("%s", buffer);
    OutputDebugStringA(buffer);
}

// TODO: This probably needs to be part if an IO file, maybe win32_file? idk.
static String8
read_stdin(Arena* arena){
    u8* str = push_array(arena, u8, KB(1));
    fgets((char*)str, KB(1), stdin);
    u64 length = str_length((char*)str);
    pop_array(arena, u8, (KB(1)-length-1));
    String8 result = str8(str, length);

    return(result);
}

///////////////////////////////
// NOTE: Win32 File Paths
///////////////////////////////

// UNTESTED: test this with a malloc so its not zero'ed out
// and you can tell that you set a null terminator
// TODO: need none OS dependant version of this in string file
static String16
os_utf8_utf16(Arena* arena, String8 utf8_string){
    u32 utf16_size = MultiByteToWideChar(CP_UTF8, 0, (char*)utf8_string.str, utf8_string.size, 0, 0);
    u16* str = (u16*)push_array(arena, u16, utf16_size + 1);
    String16 result = {str, utf16_size};

    u32 size = MultiByteToWideChar(CP_UTF8, 0, (char*)utf8_string.str, utf8_string.size, (wchar*)result.str, result.size);

    u16* null_char = result.str + result.size;
    *null_char = 0;

    return(result);
}

// UNTESTED: test this with a malloc so its not zero'ed out
// and you can tell that you set a null terminator
// TODO: need none OS dependant version of this in string file
static String8
os_utf16_utf8(Arena* arena, String16 utf16_string){
    u32 utf8_size =  WideCharToMultiByte(CP_UTF8, 0, (wchar*)utf16_string.str, utf16_string.size, 0, 0, 0, 0);
    u8* utf8_str = (u8*)push_array(arena, u8, utf8_size + 1);
    String8 result = {utf8_str, utf8_size};

    u32 size = WideCharToMultiByte(CP_UTF8, 0, (wchar*)utf16_string.str, utf16_string.size, (char*)result.str, result.size, 0, 0);

    u8* null_char = result.str + result.size;
    *null_char = 0;

    return(result);
}

static String8
os_get_cwd(Arena* arena){
    ScratchArena scratch = begin_scratch(0);
    u32 length = GetCurrentDirectoryW(0, 0);
    wchar* buffer = push_array(scratch.arena, wchar, length);

    length = GetCurrentDirectoryW(length, buffer);
    String16 utf16_string = {(u16*)buffer, length};

    String8 result = os_utf16_utf8(arena, utf16_string);
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
    u64 size;
} FileData;

// TODO IMPORTANT: Currently doesn't support large file sizes. Should fix soon.
// TODO: If I give an empty dir, it uses CWD.
// TODO: Better error handling on failures? Maybe include a DWORD error in FileData, maybe pass in FileData and only return DWORD, maybe think about overall better logging? assert? idk have to ask
static FileData
os_file_read(Arena* arena, String8 dir, String8 file_name){
    FileData result = ZERO_INIT;
    ScratchArena scratch = begin_scratch(0);
    String8 full_path = str8_concatenate(scratch.arena, dir, file_name);
    String16 wide_path = os_utf8_utf16(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ, 0, OPEN_ALWAYS, 0, 0);
    if(file_handle == INVALID_HANDLE_VALUE){
        DWORD err = GetLastError();
        print("os_file_read: failed to create file handle - error: %d\n", err);
        return(result);
    }

    LARGE_INTEGER LARGE_file_size;
    if(!GetFileSizeEx(file_handle, &LARGE_file_size)){
        print("os_file_read: failed to get file size\n");
        CloseHandle(file_handle);
        return(result);
    }

    u32 file_size = (u32)LARGE_file_size.QuadPart;
    result.base = push_array(arena, u8, file_size);
    DWORD bytes_read;
    if(!ReadFile(file_handle, result.base, file_size, &bytes_read, 0)){
        print("os_file_read: failed to read file\n");
        CloseHandle(file_handle);
        return(result);
    }

    if(bytes_read != file_size){
        print("os_file_read: bytes_read != file_size\n");
        CloseHandle(file_handle);
        return(result);
    }

    result.size = file_size;
    CloseHandle(file_handle);
    end_scratch(scratch);
    return(result);
}

// UNTESTED: offset untested
static bool
os_file_write(FileData data, String8 dir, String8 file_name, u64 offset){
    bool result = false;
    ScratchArena scratch = begin_scratch(0);
    String8 full_path = str8_concatenate(scratch.arena, dir, file_name);
    String16 wide_path = os_utf8_utf16(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_WRITE, 0, 0, OPEN_ALWAYS, 0, 0);
    if(file_handle == INVALID_HANDLE_VALUE){
        print("os_file_write: failed to create file handle\n");
        return(result);
    }

    DWORD bytes_written;
    OVERLAPPED overlapped = {
        .Offset = (DWORD)(offset & 0x00000000FFFFFFFF),
        .OffsetHigh = (DWORD)(offset >> 32)
    };
    if(!WriteFile(file_handle, data.base, data.size, &bytes_written, &overlapped)){
        DWORD err = GetLastError();
        print("os_file_write: failed to write data to file\n");
        CloseHandle(file_handle);
        return(result);
    }

    result = (data.size == bytes_written);
    CloseHandle(file_handle);
    end_scratch(scratch);
    return(result);
}

///////////////////////////////
// NOTE: Win32 File Operations
///////////////////////////////
//
// TODO: file exists
// TODO: list out files in directroy

static bool
os_file_delete(String8 dir, String8 file_name){
    ScratchArena scratch = begin_scratch(0);
    String8 full_path = str8_concatenate(scratch.arena, dir, file_name);
    String16 wide_path = os_utf8_utf16(scratch.arena, full_path);

    bool result = DeleteFileW((wchar*)wide_path.str);
    end_scratch(scratch);
    return(result);
}

static bool
os_file_move(String8 source_dir, String8 source_file, String8 dest_dir, String8 dest_file){
    ScratchArena scratch = begin_scratch(0);
    String8 source_string = str8_concatenate(scratch.arena, source_dir, source_file);
    String8 dest_string = str8_concatenate(scratch.arena, dest_dir, dest_file);
    String16 source_wide = os_utf8_utf16(scratch.arena, source_string);
    String16 dest_wide = os_utf8_utf16(scratch.arena, dest_string);

    bool result = MoveFileW((wchar*)source_wide.str, (wchar*)dest_wide.str);
    end_scratch(scratch);
    return(result);
}

static bool
os_dir_create(String8 dir, String8 new_dir){
    ScratchArena scratch = begin_scratch(0);
    String8 dir_path = str8_concatenate(scratch.arena, dir, new_dir);
    String16 wide_path = os_utf8_utf16(scratch.arena, dir_path);

    bool result = CreateDirectoryW((wchar*)wide_path.str, 0);
    end_scratch(scratch);
    return(result);
}

static bool
os_dir_delete(String8 dir, String8 delete_dir){
    ScratchArena scratch = begin_scratch(0);
    String8 dir_path = str8_concatenate(scratch.arena, dir, delete_dir);
    String16 wide_path = os_utf8_utf16(scratch.arena, dir_path);

    bool result = RemoveDirectoryW((wchar*)wide_path.str);
    end_scratch(scratch);
    return(result);
}

// list out the files in a directory
#endif
