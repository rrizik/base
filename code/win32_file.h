#if !defined(WIN32_FILE_H)
#define WIN32_FILE_H

#include <Windows.h>
#include "base_types.h"
#include "base_memory.h"
#include "base_string.h"
#include "win32_logging.h"

// note: All functions must 0 terminal strings when making allocations
// todo: Test that all functions 0 terminal strings
// todo: WIN32_FILE_ATTRIBUTE_DATA to get file attributes


// todo: This probably needs to be part if an IO file, maybe win32_io? idk.
// TODO:: add String8 to File struct
static String8
read_stdin(Arena* arena){
    u8* str = push_array(arena, u8, KB(1));

    fgets((char*)str, KB(1), stdin);
    u64 length = char_length((char*)str);

    pop_array(arena, u8, (u32)(KB(1)-length-1));
    String8 result = str8(str, length);

    return(result);
}

///////////////////////////////
// note: Win32 UTF8-16
///////////////////////////////

// untested: test this with a malloc so its not zero'ed out
// and you can tell that you set a null terminator
// todo: need none OS dependant version of this in string file
static String16
os_utf16_from_utf8(Arena* arena, String8 utf8_string){
    u32 utf16_size = (u32)MultiByteToWideChar(CP_UTF8, 0, (char*)utf8_string.str, (s32)utf8_string.size, 0, 0);
    u16* str = (u16*)push_array(arena, u16, utf16_size + 1);
    String16 result = {str, utf16_size};

    s32 size = MultiByteToWideChar(CP_UTF8, 0, (char*)utf8_string.str, (s32)utf8_string.size, (wchar*)result.str, (s32)result.size);

    u16* null_char = result.str + result.size;
    *null_char = 0;

    return(result);
}

// untested: test this with a malloc so its not zero'ed out
// and you can tell that you set a null terminator
// todo: need none OS dependant version of this in string file
static String8
os_utf8_from_utf16(Arena* arena, String16 utf16_string){
    s32 utf8_size =  WideCharToMultiByte(CP_UTF8, 0, (wchar*)utf16_string.str, (s32)utf16_string.size, 0, 0, 0, 0);
    u8* utf8_str = (u8*)push_array(arena, u8, (u32)(utf8_size + 1));
    String8 result = {utf8_str, (u64)utf8_size};

    s32 size = WideCharToMultiByte(CP_UTF8, 0, (wchar*)utf16_string.str, (s32)utf16_string.size, (char*)result.str, (s32)result.size, 0, 0);

    u8* null_char = result.str + result.size;
    *null_char = 0;

    return(result);
}

///////////////////////////////
// note: Win32 File I/O
///////////////////////////////

// todo: Get rid of this. Use String8 instead so that you can write out String8 Nodes, none contegious memory.

typedef struct File{
    HANDLE handle;
    u64 size;
} File;

static String8 os_get_cwd(Arena* arena){
    ScratchArena scratch = begin_scratch();
    defer(end_scratch(scratch));

    u32 length = GetCurrentDirectoryW(0, 0);
    wchar* buffer = push_array(scratch.arena, wchar, length);

    length = GetCurrentDirectoryW(length, buffer);
    String16 utf16_string = {(u16*)buffer, length};

    String8 utf8_string = os_utf8_from_utf16(arena, utf16_string);
    return(utf8_string);
}

static String8
os_application_path(Arena* arena) {
    String8 result = {0};

    ScratchArena scratch = begin_scratch();

    String16 utf16_path;
    utf16_path.str = push_array(scratch.arena, u16, 1024);
    utf16_path.size = GetModuleFileNameW(0, (wchar*)utf16_path.str, 1024);

    DWORD error = GetLastError();
    if(error == ERROR_INSUFFICIENT_BUFFER || utf16_path.size == 0){
        print_last_error(error);
        return(result);
    }

    String8 utf8_path = os_utf8_from_utf16(scratch.arena, utf16_path);

    String8Node* split_path = str8_split(scratch.arena, utf8_path, '\\');
    dll_pop_back(split_path); // remove exe

    String8Join opts = {0};
    opts.mid = str8_literal("/");
    opts.post = str8_literal("/");
    result = str8_join(arena, split_path, opts);

    end_scratch(scratch);
    return(result);
}

// todo: Why does this exist?
static File
os_application_file_open(String8 path, DWORD access_writes, DWORD operation){
    File result = {0};

    ScratchArena scratch = begin_scratch();
    String8 application_path = os_application_path(scratch.arena);
    String8 full_path = str8_path_append(scratch.arena, application_path, path);

    String16 wide_path = os_utf16_from_utf8(scratch.arena, full_path);

    result.handle = CreateFileW((wchar*)wide_path.str, access_writes, 0, 0, operation, 0, 0);
    if(result.handle == INVALID_HANDLE_VALUE){
        print_last_error(GetLastError());
        return(result);
    }

    LARGE_INTEGER large_file_size;
    if(!GetFileSizeEx(result.handle, &large_file_size)){
        print_last_error(GetLastError());
        return(result);
    }

    result.size = (u64)large_file_size.QuadPart;

    end_scratch(scratch);
    return(result);
}

// note(rr): Some basic access_write's to use, read docs if you want more granular access to file handles:
// access_writes: GENERIC_WRITE, GENERIC_READ
// operation:     CREATE_ALWAYS, CREATE_NEW, OPEN_ALWAYS, OPEN_EXISTING, TRUNCATE_EXISTING
static File
os_file_open(String8 path, DWORD access_writes, DWORD operation){
    File result = {0};

    ScratchArena scratch = begin_scratch();

    String16 wide_path = os_utf16_from_utf8(scratch.arena, path);
    result.handle = CreateFileW((wchar*)wide_path.str, access_writes, 0, 0, operation, 0, 0);
    if(result.handle == INVALID_HANDLE_VALUE){
        print_last_error(GetLastError());
        return(result);
    }

    LARGE_INTEGER large_file_size;
    if(!GetFileSizeEx(result.handle, &large_file_size)){
        print_last_error(GetLastError());
        return(result);
    }

    result.size = (u64)large_file_size.QuadPart;

    end_scratch(scratch);
    return(result);
}

static bool
os_file_close(File file){
    bool result = CloseHandle(file.handle);
    if(!result){
        print_last_error(GetLastError());
        return(result);
    }

    file.size = 0;
    return(result);
}

// todo IMPORTANT: Currently doesn't support large file sizes. Should fix soon.
// todo: Better error handling on failures? Maybe include a DWORD error in File, maybe pass in File and only return DWORD, maybe think about overall better logging? assert? idk have to ask
static String8
os_file_read(Arena* arena, File file){
    String8 result = {0};

    LARGE_INTEGER large_file_size;
    if(!GetFileSizeEx(file.handle, &large_file_size)){
        print_last_error(GetLastError());
        return(result);
    }

    u64 size = (u64)large_file_size.QuadPart;
    result.str = push_array(arena, u8, size);
    DWORD bytes_read = 0;
    if(!ReadFile(file.handle, result.str, (DWORD)size, &bytes_read, 0)){
        print_last_error(GetLastError());
        return(result);
    }

    bool match = (bytes_read == size);
    result.size = match ? size : 0;

    if(!match){
        print("%s(%i) error(%i): ", __FILE__, __LINE__, match);
        print("bytes_read != size: (%i, %i)\n", bytes_read, size);
        return(result);
    }
    return(result);
}

// untested:
static bool
os_file_write(File file, void* base, u64 size){
    bool result = false;
    if(file.handle == INVALID_HANDLE_VALUE){
        return(result);
    }

    DWORD bytes_written;
    // warning: Why is this commented out? What did I learn here to make me do it? I don't know
    //OVERLAPPED overlapped = {
    //    .Offset = (DWORD)(file.size & 0x00000000FFFFFFFF),
    //    .OffsetHigh = (DWORD)(file.size >> 32)
    //};
    OVERLAPPED overlapped = {0};
    overlapped.Offset = (DWORD)(file.size & 0x00000000FFFFFFFF);
    overlapped.OffsetHigh = (DWORD)(file.size >> 32);
    if(!WriteFile(file.handle, base, (DWORD)size, &bytes_written, &overlapped)){
        print_last_error(GetLastError());
        return(result);
    }

    if(size == bytes_written){
        result = true;
        file.size += size;
    }
    return(result);
}

// untested
static bool
os_file_create(String8 dir, String8 filename){
    bool result = false;

    ScratchArena scratch = begin_scratch();
    defer(end_scratch(scratch));

    String8 full_path = str8_path_append(scratch.arena, dir, filename);
    String16 wide_path = os_utf16_from_utf8(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_READ|GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    defer(CloseHandle(file_handle));
    if(file_handle == INVALID_HANDLE_VALUE){
        print_last_error(GetLastError());
        return(result);
    }

    result = true;
    return(result);
}

static bool
os_path_exists(String8 path){
    bool result = false;

    ScratchArena scratch = begin_scratch();

    String16 wide_path = os_utf16_from_utf8(scratch.arena, path);
    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_READ|GENERIC_WRITE, 0, 0, OPEN_EXISTING, 0, 0);
    defer(CloseHandle(file_handle));

    if(file_handle == INVALID_HANDLE_VALUE){
        print_last_error(GetLastError());
        return(result);
    }

    result = true;
    end_scratch(scratch);
    return(result);
}

// INCOMPLETE
static bool
os_file_exists(String8 path, String8 filename){
    bool result = false;

    ScratchArena scratch = begin_scratch();
    String8 full_path = str8_path_append(scratch.arena, path, filename);
    String16 wide_path = os_utf16_from_utf8(scratch.arena, full_path);

    HANDLE file_handle = CreateFileW((wchar*)wide_path.str, GENERIC_READ, 0, 0, OPEN_EXISTING, 0, 0);

    if(file_handle != INVALID_HANDLE_VALUE){
        result = true;
        CloseHandle(file_handle);
    }

    end_scratch(scratch);
    return(result);
}

static bool
os_file_delete(String8 path, String8 filename){
    ScratchArena scratch = begin_scratch();

    String8 full_path = str8_path_append(scratch.arena, path, filename);
    String16 wide_path = os_utf16_from_utf8(scratch.arena, full_path);

    bool result = DeleteFileW((wchar*)wide_path.str);
    end_scratch(scratch);
    return(result);
}

static bool
os_file_move(String8 source_path, String8 source_file, String8 dest_path, String8 dest_file){
    ScratchArena scratch = begin_scratch();
    String8 source_string = str8_path_append(scratch.arena, source_path, source_file);
    String8 dest_string = str8_path_append(scratch.arena, dest_path, dest_file);
    String16 source_wide = os_utf16_from_utf8(scratch.arena, source_string);
    String16 dest_wide = os_utf16_from_utf8(scratch.arena, dest_string);

    bool result = MoveFileW((wchar*)source_wide.str, (wchar*)dest_wide.str);
    end_scratch(scratch);
    return(result);
}

static bool
os_dir_create(String8 path){
    ScratchArena scratch = begin_scratch();
    defer(end_scratch(scratch));

    String16 wide_path = os_utf16_from_utf8(scratch.arena, path);
    bool result = CreateDirectoryW((wchar*)wide_path.str, 0);
    return(result);
}

static bool
os_dir_delete(String8 dir, String8 delete_dir){
    ScratchArena scratch = begin_scratch();
    String8 dir_path = str8_path_append(scratch.arena, dir, delete_dir);
    String16 wide_path = os_utf16_from_utf8(scratch.arena, dir_path);

    bool result = RemoveDirectoryW((wchar*)wide_path.str);
    end_scratch(scratch);
    return(result);
}

// untested
static bool
os_dir_files(Arena* arena, String8Node* node, String8 dir){
    bool result = false;

    ScratchArena scratch = begin_scratch();
    defer(end_scratch(scratch));

    String8 dir_slash = str8_concatenate(scratch.arena, dir, str8_literal("\\*"));
    String16 dir_utf16 = os_utf16_from_utf8(scratch.arena, dir_slash);

    WIN32_FIND_DATAW data = {0};
    HANDLE file_handle = FindFirstFileW((wchar*)dir_utf16.str, &data);
    defer(CloseHandle(file_handle));

    if(file_handle == INVALID_HANDLE_VALUE){
        print_last_error(GetLastError());
        return(result);
    }

    do{
        u64 length = wchar_length(data.cFileName);
        String16 string_utf16 = {(u16*)data.cFileName, length};
        String8 string_utf8 = os_utf8_from_utf16(scratch.arena, string_utf16); // todo: test this
        str8_list_push_back(arena, node, string_utf8);
        print("FILE: %s\n", string_utf8.str);
    }while(FindNextFileW(file_handle, &data));

    result = true;
    return(result);
}

#endif
