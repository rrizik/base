#if !defined(WIN32_FILE_H)
#define WIN32_FILE_H

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "base_types.h"
#include "base_memory.h"
#include "base_string.h"
#include "win32_logging.h"

// TODO: WIN32_FILE_ATTRIBUTE_DATA to get file attributes
// TODO: UNTESTED: Retest this entire file


// TODO: This probably needs to be part if an IO file, maybe win32_io? idk.
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
// NOTE: Win32 UTF8-16
///////////////////////////////

// UNTESTED: test this with a malloc so its not zero'ed out
// and you can tell that you set a null terminator
// TODO: need none OS dependant version of this in string file
static String16
os_utf8_utf16(Arena* arena, String8 utf8_string){
    u32 utf16_size = (u32)MultiByteToWideChar(CP_UTF8, 0, (char*)utf8_string.str, (s32)utf8_string.size, 0, 0);
    u16* str = (u16*)push_array(arena, u16, utf16_size + 1);
    String16 result = {str, utf16_size};

    s32 size = MultiByteToWideChar(CP_UTF8, 0, (char*)utf8_string.str, (s32)utf8_string.size, (wchar*)result.str, (s32)result.size);

    u16* null_char = result.str + result.size;
    *null_char = 0;

    return(result);
}

// UNTESTED: test this with a malloc so its not zero'ed out
// and you can tell that you set a null terminator
// TODO: need none OS dependant version of this in string file
static String8
os_utf16_utf8(Arena* arena, String16 utf16_string){
    s32 utf8_size =  WideCharToMultiByte(CP_UTF8, 0, (wchar*)utf16_string.str, (s32)utf16_string.size, 0, 0, 0, 0);
    u8* utf8_str = (u8*)push_array(arena, u8, (u32)(utf8_size + 1));
    String8 result = {utf8_str, (u64)utf8_size};

    s32 size = WideCharToMultiByte(CP_UTF8, 0, (wchar*)utf16_string.str, (s32)utf16_string.size, (char*)result.str, (s32)result.size, 0, 0);

    u8* null_char = result.str + result.size;
    *null_char = 0;

    return(result);
}

// path to binary/.exe
// path for user config/data
// path where temporary files can go

///////////////////////////////
// NOTE: Win32 File I/O
///////////////////////////////

// TODO: Get rid of this. Use String8 instead so that you can write out String8 Nodes, none contegious memory.

typedef struct File{
    HANDLE handle;
    u64 size;
} File;

static String8
os_get_cwd(Arena* arena){
    ScratchArena scratch = begin_scratch(0);
    defer(end_scratch(scratch));

    u32 length = GetCurrentDirectoryW(0, 0);
    wchar* buffer = push_array(scratch.arena, wchar, length);

    length = GetCurrentDirectoryW(length, buffer);
    String16 utf16_string = {(u16*)buffer, length};

    String8 utf8_string = os_utf16_utf8(arena, utf16_string);
    return(utf8_string);
}

// UNTESTED do I need to pass back a pointer? Its not clear if this data will persist outside this scope
static File
os_file_open(String8 path, DWORD access_writes, DWORD operation){
    File result = {0};

    ScratchArena scratch = begin_scratch(0);
    String16 wide_path = os_utf8_utf16(scratch.arena, path);
    defer(end_scratch(scratch));

    result.handle = CreateFileW((wchar*)wide_path.str, access_writes, 0, 0, operation, 0, 0);
    if(result.handle){
        print_last_error(GetLastError());
        return(result);
    }

    LARGE_INTEGER large_file_size;
    if(!GetFileSizeEx(result.handle, &large_file_size)){
        print_last_error(GetLastError());
        return(result);
    }

    result.size = (u64)large_file_size.QuadPart;

    return(result);
}

static bool
os_file_close(File* file){
    bool result = CloseHandle(file->handle);
    if(!result){
        print_last_error(GetLastError());
        return(result);
    }

    file->size = 0;
    return(result);
}

// TODO IMPORTANT: Currently doesn't support large file sizes. Should fix soon.
// TODO: Better error handling on failures? Maybe include a DWORD error in File, maybe pass in File and only return DWORD, maybe think about overall better logging? assert? idk have to ask
static String8
os_file_read(Arena* arena, File* file){
    String8 result = {0};

    LARGE_INTEGER large_file_size;
    if(!GetFileSizeEx(file->handle, &large_file_size)){
        print_last_error(GetLastError());
        return(result);
    }

    size_t size = (u64)large_file_size.QuadPart;
    result.str = push_array(arena, u8, size);
    DWORD bytes_read = 0;
    if(!ReadFile(file->handle, result.str, (DWORD)size, &bytes_read, 0)){
        print_last_error(GetLastError());
        return(result);
    }

    bool match = (bytes_read == size);
    result.size = match ? size : 0;

    if(!match){
        print("%s(%i) error(%i): ", __FILE__, __LINE__, match);
        print("%bytes_read != size: (%i, %i)\n", bytes_read, size);
        return(result);
    }
    return(result);
}

// UNTESTED:
static bool
os_file_write(File* file, void* base, u64 size){
    bool result = false;

    DWORD bytes_written;
    OVERLAPPED overlapped = {
        .Offset = (DWORD)(file->size & 0x00000000FFFFFFFF),
        .OffsetHigh = (DWORD)(file->size >> 32)
    };
    if(!WriteFile(file->handle, base, (DWORD)size, &bytes_written, &overlapped)){
        print_last_error(GetLastError());
        return(result);
    }

    if(size == bytes_written){
        result = true;
        file->size += size;
    }
    return(result);
}

// UNTESTED
static bool
os_file_create(String8 dir, String8 filename){
    bool result = false;

    ScratchArena scratch = begin_scratch(0);
    defer(end_scratch(scratch));

    String8 full_path = str8_path_append(scratch.arena, dir, filename);
    String16 wide_path = os_utf8_utf16(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_READ|GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if(file_handle){
        print_last_error(GetLastError());
        return(result);
    }
    CloseHandle(file_handle);

    result = true;
    return(result);
}

// INCOMPLETE
static bool
os_file_exists(String8 dir, String8 filename){
    bool result = false;
    ScratchArena scratch = begin_scratch(0);
    defer(end_scratch(scratch));

    String8 full_path = str8_path_append(scratch.arena, dir, filename);
    String16 wide_path = os_utf8_utf16(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    defer(CloseHandle(file_handle));

    if(file_handle){
        return(result);
    }

    result = true;
    return(result);
}

static bool
os_file_delete(String8 dir, String8 filename){
    ScratchArena scratch = begin_scratch(0);
    String8 full_path = str8_path_append(scratch.arena, dir, filename);
    String16 wide_path = os_utf8_utf16(scratch.arena, full_path);

    bool result = DeleteFileW((wchar*)wide_path.str);
    defer(end_scratch(scratch));
    return(result);
}

static bool
os_file_move(String8 source_dir, String8 source_file, String8 dest_dir, String8 dest_file){
    ScratchArena scratch = begin_scratch(0);
    String8 source_string = str8_path_append(scratch.arena, source_dir, source_file);
    String8 dest_string = str8_path_append(scratch.arena, dest_dir, dest_file);
    String16 source_wide = os_utf8_utf16(scratch.arena, source_string);
    String16 dest_wide = os_utf8_utf16(scratch.arena, dest_string);

    bool result = MoveFileW((wchar*)source_wide.str, (wchar*)dest_wide.str);
    end_scratch(scratch);
    return(result);
}

static bool
os_dir_create(String8 dir, String8 new_dir){
    ScratchArena scratch = begin_scratch(0);
    String8 dir_path = str8_path_append(scratch.arena, dir, new_dir);
    String16 wide_path = os_utf8_utf16(scratch.arena, dir_path);

    bool result = CreateDirectoryW((wchar*)wide_path.str, 0);
    end_scratch(scratch);
    return(result);
}

static bool
os_dir_delete(String8 dir, String8 delete_dir){
    ScratchArena scratch = begin_scratch(0);
    String8 dir_path = str8_path_append(scratch.arena, dir, delete_dir);
    String16 wide_path = os_utf8_utf16(scratch.arena, dir_path);

    bool result = RemoveDirectoryW((wchar*)wide_path.str);
    end_scratch(scratch);
    return(result);
}

// UNTESTED
static bool
os_dir_files(Arena* arena, String8Node* node, String8 dir){
    bool result = false;

    ScratchArena scratch = begin_scratch(0);
    defer(end_scratch(scratch));
    String8 dir_slash = str8_concatenate(scratch.arena, dir, str8_literal("\\*"));
    String16 dir_utf16 = os_utf8_utf16(scratch.arena, dir_slash);

    WIN32_FIND_DATAW data = {0};
    HANDLE file_handle = FindFirstFileW((wchar*)dir_utf16.str, &data);
    if(file_handle){
        DWORD err = GetLastError();
        print("os_dir_files: failed to create file handle - dir: %s - error code: %d\n", dir.str, err);
        return(result);
    }
    defer(CloseHandle(file_handle));
    result = true;

    do{
        u64 length = str_length(data.cFileName);
        String16 string_utf16 = {(u16*)data.cFileName, length};
        String8 string_utf8 = os_utf16_utf8(scratch.arena, string_utf16); // TODO: test this
        str8_list_push_back(arena, node, string_utf8);
        print("FILE: %s\n", string_utf8.str);
    }while(FindNextFileW(file_handle, &data));
    return(result);
}

static String8
os_get_exe_path(Arena* arena) {
    String8 result = {0};

    wchar buff[1024];
    u32 size = GetModuleFileNameW(0, buff, 1024); // size doesn't include null terminated character

    String16 utf16_string = {0};
    utf16_string.str = (u16*)buff;
    utf16_string.size = size;

    ScratchArena scratch = begin_scratch(0);
    String8 utf8_string = os_utf16_utf8(scratch.arena, utf16_string);
    result.str = push_array(arena, u8, size + 1); // +1 to include null terminated character
    result.size = size;
    memory_copy(result.str, utf8_string.str, size);
    end_scratch(scratch);

    return(result);
}

#endif
