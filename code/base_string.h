/*
Note String8 is an immutable type that should be considered as a "view" into data.
There are a few exeptions to this such as:
 - str8_split() // allocates memory, although it shouldn't need to
 - str8_join()  // allocates memory
 - str8_concatenate() // allocates memory
 - str8_replace_byte() // mutates existing memory

===================================================================================================
===================================================================================================

typedef struct String8;
typedef struct String8Node;
typedef struct String8List;
typedef struct String8Join;

===================================================================================================
===================================================================================================

static u64 char_length(char* buff);
static u64 char_copy(char* dst, char* src); // assumes both buffers are 0 terminated
static u64 wchar_length(wchar* wbuff);
static u64 wchar_copy(wchar* dst, wchar* src); // assumes both buffers are 0 terminated

#define        str8(byte_buffer, count) str8_((u8*)byte_buffer, count)
#define        str8_literal(byte_buffer) str8_((u8*)byte_buffer, (sizeof(byte_buffer) - 1))
static String8 str8_(u8* byte_buffer, u64 count);
static String8 str8_range(u8* first, u8* opl);
static String8 str8_cstring(char* cstr);
static String8 str8_format(Arena* arena, const char* format, ...);

#define str16(string, count) str16_((u16*)string, count)
static  String16 str16_(u16* string, u64 count);

static bool    byte_is_digit(u8 byte);
static bool    byte_is_alpha(u8 byte);
static bool    byte_is_alnum(u8 byte);
static bool    byte_is_special(u8 byte);
static bool    byte_is_slash(u8 byte);
static bool    byte_is_space(u8 byte);
static bool    byte_is_upper(u8 byte);
static bool    byte_is_lower(u8 byte);
static u8      byte_to_upper(u8 byte);
static u8      byte_to_lower(u8 byte);

static void    str8_to_upper(String8* string);
static void    str8_to_lower(String8* string);
static bool    str8_is_upper(String8 string);
static bool    str8_is_lower(String8 string);
static bool    str8_is_digit(String8 string);
static bool    str8_is_alpha(String8 string);
static bool    str8_is_zero_terminated(String8 string);

static String8 str8_zero_terminate(Arena* arena, String8 string);
static String8 str8_concatenate(Arena* arena, String8 left, String8 right);
static u64     str8_replace_byte(String8* string, u8 byte_in, u8 byte_out);

static void    str8_advance(String8* str, u64 count);
static String8 str8_advance(String8 str, u64 count);
static u64     str8_eat_spaces(String8* string);
static String8 str8_eat_spaces(String8 string);
static String8 str8_eat_word(String8* string);
static String8 str8_eat_line(String8* string);

static void    str8_split_left(String8* string, u64 index);
static String8 str8_split_left(String8 string, u64 index);
static void    str8_split_right(String8* string, u64 index);
static String8 str8_split_right(String8 string, u64 index);
static void    str8_trim_left(String8* string, u64 count);
static String8 str8_trim_left(String8 string, u64 count);
static void    str8_trim_right(String8* string, u64 count);
static String8 str8_trim_right(String8 string, u64 count);

static bool    str8_compare(String8 left, String8 right);
static bool    str8_compare_nocase(String8 left, String8 right);
static bool    str8_contains(String8 string, String8 sub_string);
static bool    str8_contains_byte(String8 string, u8 byte);
static bool    str8_contains_nocase(String8 string, String8 sub_string);
static bool    str8_contains_alpha(String8 string);
static bool    str8_contains_digit(String8 string);
static bool    str8_starts_with(String8 string, String8 sub_string);
static bool    str8_starts_with_nocase(String8 string, String8 sub_string);
static bool    str8_ends_with(String8 string, String8 sub_string);
static bool    str8_ends_with_nocase(String8 string, String8 sub_string);
static bool    str8_ends_with_byte(String8 string, u8 byte); // consider: maybe I don't want this

static s64     str8_index_from_left(String8 string, u8 byte);
static s64     str8_index_from_right(String8 string, u8 byte);
static s64     str8_index_from_left(String8 string, String8 sub_string);
static s64     str8_index_from_right(String8 string, String8 sub_string);
static s64     any_index_from_right(String8 string, String8 any);
static s64     any_index_from_left(String8 string, String8 any);

// todo keep as new
static void        str8_list_push(Arena* arena, String8List* list, String8 string);
static String8     str8_join(Arena* arena, String8List* str8_sentinel, String8Join join_opts);
static String8Node str8_split(Arena* arena, String8 string, char byte, u32 count);

// todo remove as old
static void        str8_list_push_back(Arena* arena, String8Node* str_sentinel, String8 string);
static String8     str8_join(Arena* arena, String8Node* str8_sentinel, String8Join join_opts);
static String8Node str8_split(Arena* arena, String8 string, char byte);

static String8     str8_path_append(Arena* arena, String8 path, String8 value);
static String8     str8_path_pop(Arena* arena, String8 path, char slash);
static String8     str8_path_file(String8 path);
static String8     str8_path_filename(String8 path);
static String8     str8_path_extension(String8 path);
static void        str8_path_strip_file(String8* path);
static void        str8_path_strip_extension(String8 path);
static bool        str8_path_is_absolute(String8 path);
*/

#ifndef BASE_STRING_H
#define BASE_STRING_H

#include "base_types.h"
#include "base_linkedlist.h"
#include "base_memory.h"

// todo:: make all paths coming from OS forward slashed
// todo:: Write is_zero_terminated() function to check

// todo:: other useful helpers
// string_to_float:: (_arg: string) -> result: $T, success: bool, remainder: string {
// string_to_int :: (t: string, base := 10, $T := int) -> result: T, success: bool, remainder: string {
// eat_trailing_spaces :: (_s: string) -> string;



///////////////////////////////
// note: String8
///////////////////////////////

typedef struct String8{
    union{ u8* str; u8* data; u8* base; };
    union{ u64 size; u64 count; u64 length; };
} String8;

#define str8(byte_buffer, count) str8_((u8*)byte_buffer, count)
#define str8_literal(byte_buffer) str8_((u8*)byte_buffer, (sizeof(byte_buffer) - 1))
#define str8_lit(byte_buffer) str8_literal(byte_buffer)
static String8
str8_(u8* byte_buffer, u64 count){
    String8 result = {byte_buffer, count};
    return(result);
}

static String8
str8_range(u8* first, u8* opl){
    String8 result = {first, (u64)(opl - first)};
    return(result);
}

static String8
str8_cstring(char* cstr){
    char* ptr = cstr;
    for(; *ptr != 0; ptr += 1);
    String8 result = str8_range((u8*)cstr, (u8*)ptr);
    return(result);
}

#include <stdio.h>
#include <stdarg.h>
#define str8_fmt(arena, format, ...) str8_formatted(arena, format, __VA_ARGS__)
#define str8_format(arena, format, ...) str8_formatted(arena, format, __VA_ARGS__)
static String8
str8_formatted(Arena* arena, const char* format, ...){
    char buffer[4096] = {};
    va_list args;
    va_start(args, format);
    u64 count = (u64)vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    u8* byte_buffer = (u8*)push_array(arena, u8, count);
    memcpy(byte_buffer, buffer, count);
    String8 result = str8(byte_buffer, count);

    return(result);
}

typedef struct String8Node{
    String8Node* next;
    String8Node* prev;
    String8 string;
} String8Node;

typedef struct String8List{
    String8Node* first;
    String8Node* last;
    u64 node_count;
    u64 total_size;
} String8List;

//static String8Node*
//push_str8_node(Arena* arena){
//    String8Node* result = push_struct(arena, String8Node);
//    result->next = result;
//    result->prev = result;
//    return(result);
//}

typedef struct String8Join{
    String8 pre;
    String8 mid;
    String8 post;
} String8Join;

typedef union String16{
    struct{ u16* str; u64 size; };
    struct{ u16* data; u64 count; };
} String16;

#define str16(string, count) str16_((u16*)string, count)
static String16 str16_(u16* string, u64 count){
    String16 result = {string, count};
    return(result);
}

///////////////////////////////
// note: Char helpers
///////////////////////////////
// important: All functions must 0 terminal strings when making allocations

static u64 char_length(char* buff){
    u64 count = 0;
    while(*buff++){
        ++count;
    }
    buff = buff - count;
    return(count);
}

static u64 char_copy(char* dst, char* src){
    u64 count = 0;
    while(*src){
        *dst = *src;
        dst++;
        src++;
        count++;
    }
    *dst = '\0';
    src = src - count;
    dst = dst - count;
    return(count);
}

static u64 wchar_length(wchar* wbuff){
    u64 count = 0;
    while(*wbuff++){
        ++count;
    }
    wbuff = wbuff - count;
    return(count);
}

static u64 wchar_copy(wchar* dst, wchar* src){
    u64 count = 0;
    while(*src){
        *dst = *src;
        src++;
        dst++;
        count++;
    }
    *dst = '\0';
    dst = dst - count;
    src = src - count;
    return(count);
}


///////////////////////////////
// NOTE: String8 Basic
///////////////////////////////

static bool
byte_is_digit(u8 byte){
    bool result = ((byte >= '0') && (byte <= '9'));
    return(result);
}

static bool
byte_is_alpha(u8 byte){
    if((byte >= 'a' && byte <= 'z') || (byte >= 'A' && byte <= 'Z')){
        return(true);
    }
    return(false);
}

static bool
byte_is_alnum(u8 byte){
    if(byte_is_alpha(byte) || byte_is_digit(byte)){
        return(true);
    }
    return(false);
}

static bool
byte_is_special(u8 byte){
    if(byte_is_alnum(byte)){
        return(false);
    }

    if(byte >= 32 && byte <= 126){
        return(true);
    }
    return(false);
}

static bool
byte_is_slash(u8 byte){
    return(byte == '\\' || byte == '/');
}

static bool
byte_is_space(u8 byte){
    if(byte == ' ' || byte == '\n' || byte == '\t' || byte == '\r'){
        return(true);
    }
    return(false);
}

static bool
byte_is_upper(u8 byte){
    if(byte >= 'A' && byte <= 'Z'){
        return(true);
    }
    return(false);
}

static bool
byte_is_lower(u8 byte){
    if(byte >= 'a' && byte <= 'z'){
        return(true);
    }
    return(false);
}

static u8
byte_to_upper(u8 byte){
    if(byte >= 'a' && byte <= 'z'){
        return((u8)(byte + ('A' - 'a')));
    }
    return(byte);
}

static u8
byte_to_lower(u8 byte){
    if(byte >= 'A' && byte <= 'Z'){
        return((u8)(byte + 'a' - 'A'));
    }
    return(byte);
}

static bool
str8_is_upper(String8 string){
    for(s32 i=0; i < string.count; ++i){
        if(byte_is_lower(string.data[i])){
            return(false);
        }
    }
    return(true);
}

static bool
str8_is_lower(String8 string){
    for(s32 i=0; i < string.count; ++i){
        if(byte_is_upper(string.data[i])){
            return(false);
        }
    }
    return(true);
}

static void
str8_to_upper(String8* string){
    for(s32 i=0; i < string->count; ++i){
        string->data[i] = byte_to_upper(string->data[i]);
    }
}

static void
str8_to_lower(String8* string){
    for(s32 i=0; i < string->count; ++i){
        string->data[i] = byte_to_lower(string->data[i]);
    }
}

static bool
str8_is_digit(String8 string){
    bool result = true;
    for(s32 i=0; i < string.count; ++i){
        if(!byte_is_digit(string.data[i])){
            result = false;
            break;
        }
    }
    return(result);
}

static bool
str8_is_alpha(String8 string){
    bool result = true;
    for(s32 i=0; i < string.count; ++i){
        if(!byte_is_alpha(string.data[i])){
            result = false;
            break;
        }
    }
    return(result);
}

static bool
str8_contains_alpha(String8 string){
    bool result = false;
    for(s32 i=0; i < string.count; ++i){
        if(byte_is_alpha(string.data[i])){
            result = true;
            break;
        }
    }
    return(result);
}

static bool
str8_contains_digit(String8 string){
    bool result = false;
    for(s32 i=0; i < string.count; ++i){
        if(byte_is_digit(string.data[i])){
            result = true;
            break;
        }
    }
    return(result);
}

static bool
str8_is_zero_terminated(String8 string){
    if(string.data[string.count - 1] == '\0'){
        return(true);
    }
    return(false);
}

static String8
str8_zero_terminate(Arena* arena, String8 string){
    u8* buffer = push_array(arena, u8, string.count + 1);
    memcpy(buffer, string.data, string.count + 1);

    String8 result = {0};
    result.data = buffer;
    result.count = string.count + 1;

    u8* last = buffer + string.count;
    *last = 0;

    return(result);
}

#define str8_concat(arena, left, right) str8_concatenate(arena, left, right)
static String8
str8_concatenate(Arena* arena, String8 left, String8 right){
    u8* buffer = (u8*)push_array(arena, u8, (left.count + right.count));
    String8 result = {buffer, (left.count + right.count)};

    for(u64 i = 0; i < left.count; ++i){
        *buffer++ = *(left.data + i);
    }
    for(u64 i = 0; i < right.count; ++i){
        *buffer++ = *right.data++;
    }

    return(result);
}

static u64
str8_replace_byte(String8* string, u8 byte_in, u8 byte_out){
    u64 count = 0;
    for(s32 i=0; i < string->count; ++i){
        if(string->data[i] == byte_in){
            string->data[i] = byte_out;
            ++count;
        }
    }
    return(count);
}

static String8
str8_advance(String8 string, u64 count){
    if(string.count >= count){
        string.data = string.data + count;
        string.count -= count;
    }
    return(string);
}

static void
str8_advance(String8* string, u64 count){
    if(string->count >= count){
        string->data = string->data + count;
        string->count -= count;
    }
}

// CONSIDER: Maybe have other functions like this, like eat_digits, eat_number, eat_string, ...
static String8
str8_eat_spaces(String8 string){
    while(string.count){
        if((string.data[0] != ' ') && (string.data[0] != '\t') && (string.data[0] != '\n') && (string.data[0] != '\r') && (string.data[0] != '\v') && (string.data[0] != '\f')){
            break;
        }
        string = str8_advance(string, 1);
    }
    return(string);
}

static u64
str8_eat_spaces(String8* string){
    u64 count = 0;
    while(string->count){
        if((string->data[0] != ' ') && (string->data[0] != '\t') && (string->data[0] != '\n') && (string->data[0] != '\r') && (string->data[0] != '\v') && (string->data[0] != '\f')){
            break;
        }
        str8_advance(string, 1);
        count++;
    }
    return(count);
}

static String8
str8_eat_word(String8* string){
    String8 result = {0};
    str8_eat_spaces(string);

    u64 count = 0;
    while(string->count){
        if(byte_is_space(*string->data)){
            break;
        }
        str8_advance(string, 1);
        ++count;
    }

    result = {string->data - count, count};
    return(result);
}

static String8
str8_eat_line(String8* string){
    String8 result = {0};

    u64 count = 0;
    while(string->count){
        if(*string->data == '\n'){
            str8_advance(string, 1); // consume the \n
            ++count;
            break;
        }
        str8_advance(string, 1);
        ++count;
    }

    result = {string->data - count, count};
    return(result);
}

static void
str8_split_left(String8* string, u64 index){
    if(string->count >= index){
        string->count = index;
    }
}

static String8
str8_split_left(String8 string, u64 index){
    if(string.count >= index){
        string.count = index;
    }
    return(string);
}

static void
str8_split_right(String8* string, u64 index){
    if(string->count >= index){
        string->data = string->data + (index + 1);
        string->count -= (index + 1);
    }
}

static String8
str8_split_right(String8 string, u64 index){
    if(string.count >= index){
        string.data = string.data + (index + 1);
        string.count -= (index + 1);
    }
    return(string);
}

// consider: str8_trim_both_sides_at_the_same_time()? or str8_sub_string()?
static void
str8_trim_left(String8* string, u64 count){
    u64 min = MIN(string->count, count);
    string->data = string->data + min;
    string->count -= min;
}

static String8
str8_trim_left(String8 string, u64 count){
    u64 min = MIN(string.count, count);
    String8 result = {string.data + min, string.count - min};
    return(result);
}

static void
str8_trim_right(String8* string, u64 count){
    u64 min = MIN(string->count, count);
    string->count -= min;
}

static String8
str8_trim_right(String8 string, u64 count){
    u64 min = MIN(string.count, count);
    String8 result = {string.data, string.count - min};
    return(result);
}

///////////////////////////////
// NOTE: String8 Non-Basic
///////////////////////////////

static bool
str8_compare(String8 left, String8 right){
    if(left.count != right.count){
        return(false);
    }

    for(u64 i=0; i < left.count; ++i){
        if(left.data[i] != right.data[i]){
            return(false);
        }
    }

    return(true);
}

static bool
str8_compare_nocase(String8 left, String8 right){
    if(left.count != right.count){
        return(false);
    }

    for(u64 i=0; i < left.count; ++i){
        if(byte_to_lower(left.data[i]) != byte_to_lower(right.data[i])){
            return(false);
        }
    }

    return(true);
}

static bool
str8_contains(String8 string, String8 sub_string){
    if(sub_string.count > string.count){
        return(false);
    }

    u64 count = 0;
    for(u64 i=0; i < string.count; ++i){
        if(string.data[i] == sub_string.data[count]){
            count++;
        }
        else{
            count = 0;
        }

        if(count == sub_string.count){
            return(true);
        }
    }
    return(false);
}

static bool
str8_contains_byte(String8 string, u8 byte){
    for(u64 i=0; i < string.count; ++i){
        if(string.data[i] == byte){
            return(true);
        }
    }
    return(false);
}

static bool
str8_contains_nocase(String8 string, String8 sub_string){
    u64 count = 0;
    for(u64 i=0; i < string.count; ++i){
        if(byte_to_lower(string.data[i]) == byte_to_lower(sub_string.data[count])){
            count++;
        }
        else{
            count = 0;
        }

        if(count == sub_string.count){
            return(true);
        }
    }
    return(false);
}

static bool
str8_starts_with(String8 string, String8 sub_string){
    if(sub_string.count > string.count){
        return(false);
    }
    for(u64 i=0; i < sub_string.count; ++i){
        if(string.data[i] != sub_string.data[i]){
            return(false);
        }
    }
    return(true);
}

static bool
str8_starts_with_nocase(String8 string, String8 sub_string){
    if(sub_string.count > string.count){
        return(false);
    }
    for(u64 i=0; i < sub_string.count; ++i){
        if(byte_to_lower(string.data[i]) != byte_to_lower(sub_string.data[i])){
            return(false);
        }
    }
    return(true);
}

static bool
str8_ends_with(String8 string, String8 sub_string){
    if(sub_string.count > string.count){
        return(false);
    }

    u64 index = string.count;
    for(u64 i=sub_string.count; i <= sub_string.count; --i){
        if(string.data[index] != sub_string.data[i]){
            return(false);
        }
        --index;
    }
    return(true);
}

static bool
str8_ends_with_byte(String8 string, u8 byte){
    if(string.count < 1){
        return(false);
    }

    if(string.str[string.count - 1] == byte){
        return(true);
    }
    return(false);
}

static bool
str8_ends_with_nocase(String8 string, String8 sub_string){
    if(sub_string.count > string.count){
        return(false);
    }

    u64 index = string.count;
    for(u64 i=sub_string.count; i <= sub_string.count; --i){
        if(byte_to_lower(string.data[index]) != byte_to_lower(sub_string.data[i])){
            return(false);
        }
        --index;
    }
    return(true);
}

#define byte_index_from_left(a, b) str8_index_from_left(a, b)
static s64
str8_index_from_left(String8 string, u8 byte){
    s64 index = 0;
    while(index < (s64)string.count){
        if(string.data[index] == byte){
            return((s64)index);
        }
        index++;
    }
    return(-1);
}

static s64
str8_index_from_right(String8 string, u8 byte){
    u64 index = string.count - 1;
    while(index < string.count){
        if(string.data[index] == byte){
            return((s64)index);
        }
        --index;
    }
    return(-1);
}

static s64
str8_index_from_left(String8 string, String8 sub_string){
    if(string.count < sub_string.count){
        return(-1);
    }

    bool first = true;
    s64 index = -1;
    s64 sub_index = 0;
    for(u64 i=0; i < string.count; ++i){
        if(string.data[i] == sub_string.data[sub_index]){
            // Not enough bytes left to check.
            if(string.count - i < sub_string.count){
                return(-1);
            }

            index = (s64)i;
            for(u64 j=0; j < sub_string.count; ++j){
                if(string.data[i + j] == sub_string.data[sub_index]){
                    ++sub_index;
                }
                else{
                    index = -1;
                    sub_index = 0;
                    break;
                }

                if(sub_index == (s64)sub_string.count){
                    return((s64)index);
                }
            }
        }
    }
    return(-1);
}

static s64
str8_index_from_right(String8 string, String8 sub_string){
    if(string.count < sub_string.count){
        return(-1);
    }

    bool first = true;
    s64 index = -1;
    u64 sub_index = sub_string.count - 1;
    for(u64 i=string.count - 1; i < string.count; --i){
        if(string.data[i] == sub_string.data[sub_index]){
            // Not enough bytes left to check.
            if(string.count - (string.count - i) < sub_string.count - 1){
                return(-1);
            }

            index = (s64)(string.count - i);
            for(u64 j=0; j < sub_string.count; ++j){
                if(string.data[i - j] == sub_string.data[sub_index]){
                    --sub_index;
                }
                else{
                    index = -1;
                    sub_index = 0;
                    break;
                }

                if(sub_index == -1){
                    return((s64)index);
                }
            }
        }
    }
    return(-1);
}

static s64
any_index_from_right(String8 string, String8 any){
    s64 index = -1;
    for(u64 i=0; i < any.count; ++i){
        u8 byte = any.data[i];
        index = str8_index_from_right(string, byte);
        if(index != -1){
            return(index);
        }
    }
    return(index);
}

static s64
any_index_from_left(String8 string, String8 any){
    s64 index = -1;
    for(u64 i=0; i < any.count; ++i){
        u8 byte = any.data[i];
        index = str8_index_from_left(string, byte);
        if(index != -1){
            return(index);
        }
    }
    return(index);
}

///////////////////////////////
// NOTE: String8 Path Helpers
///////////////////////////////

static String8
str8_path_file(String8 path){
    s64 index = any_index_from_right(path, str8_literal("\\/"));
    path = str8_split_right(path, (u64)index); // include / in split
    return(path);
}

static String8
str8_path_filename(String8 path){
    path = str8_path_file(path);
    s64 index = str8_index_from_right(path, '.');
    path = str8_trim_right(path, path.count - index);
    return(path);
}

static String8
str8_path_extension(String8 path){
    s64 index = str8_index_from_right(path, '.');
    path = str8_trim_left(path, (u64)index);
    return(path);
}

static void
str8_path_strip_file(String8* path){
    s64 index = any_index_from_right(*path, str8_literal("\\/"));
    str8_trim_right(path, path->count - (u64)index);
}

static void
str8_path_strip_extension(String8 path){
    s64 index = str8_index_from_right(path, '.');
    str8_trim_right(path, path.count - (u64)index);
}

static bool
str8_path_is_absolute(String8 path){
    if(path.count < 3){
        return(false);
    }

    if(path.data[1] == ':' && (path.data[2] == '/' || path.data[2] == '\\')){
        return(true);
    }

    return(false);
}

static void
str8_list_push(Arena* arena, String8List* list, String8 string){
    String8Node* node = push_struct(arena, String8Node);
    node->string = string;

    if(list->first == 0){
        list->first = node;
        list->last = node;
    }
    else{
        list->last->next = node;
        list->last = node;
    }
    node->next = 0;

    list->node_count += 1;
    list->total_size += string.size;
}

static void
str8_list_push_back(Arena* arena, String8Node* str_sentinel, String8 string){
    String8Node* string_node = push_array(arena, String8Node, 1);

    string_node->string.size = string.size;
    string_node->string.data = push_array(arena, u8, string.size);
    memcpy(string_node->string.data, string.data, string.size);

    dll_push_back(str_sentinel, string_node);
}

static String8List
str8_split(Arena* arena, String8 string, char byte, u32 count){
    String8List parts = {};

    u8* ptr   = string.data;
    u8* first = string.data;
    u8* opl   = string.data + string.size;
    for(; ptr < opl; ptr += 1){
        bool is_split_byte = false;
        if(*ptr == byte){
            is_split_byte = true;
        }

        if(is_split_byte){
            if(first < ptr){
                str8_list_push(arena, &parts, str8_range(first, ptr));
            }
            first = ptr + 1;
        }
    }

    if(first < ptr){
        str8_list_push(arena, &parts, str8_range(first, ptr));
    }

    return(parts);
}

static String8Node*
str8_split(Arena* arena, String8 string, char byte){
    String8Node* parts = push_struct(arena, String8Node);
    dll_clear(parts);

    u8* ptr =   string.data;
    u8* first = string.data;
    u8* opl =   string.data + string.size;
    for(; ptr < opl; ptr += 1){
        bool is_split_byte = false;
        if(*ptr == byte){
            is_split_byte = true;
        }

        if(is_split_byte){
            if(first < ptr){
                str8_list_push_back(arena, parts, str8_range(first, ptr));
            }
            first = ptr + 1;
        }
    }

    if(first < ptr){
        str8_list_push_back(arena, parts, str8_range(first, ptr));
    }

    return(parts);
}

static String8
str8_join(Arena* arena, String8List* list, String8Join join_opts){
    u64 size = 0;
    size += join_opts.pre.size;
    size += join_opts.post.size;
    size += join_opts.mid.size * (list->node_count - 1);
    size += list->total_size;

    // allocate count
    u8* str = push_array(arena, u8, size + 1);
    u8* ptr = str;

    // write pre
    memcpy(ptr, join_opts.pre.data, join_opts.pre.size);
    ptr += join_opts.pre.size;

    // write mid
    bool is_mid = false;
    for(String8Node* node = list->first; node != 0; node = node->next){
        if(is_mid && join_opts.mid.size){
            memcpy(ptr, join_opts.mid.data, join_opts.mid.size);
            ptr += join_opts.mid.size;
        }

        // write node string
        memcpy(ptr, node->string.str, node->string.size);
        ptr += node->string.size;

        is_mid = true;
    }

    // write post
    memcpy(ptr, join_opts.post.data, join_opts.post.size);
    ptr += join_opts.post.size;

    // write zero.
    *ptr = 0;

    String8 result = {str, size};
    return(result);
}

static String8
str8_join(Arena* arena, String8Node* str_sentinel, String8Join join_opts){
    u64 count = 0;

    // calc total count
    count += join_opts.pre.count;
    count += join_opts.post.count;
    for(String8Node* node = str_sentinel->next; node != str_sentinel; node = node->next){
        count += node->string.count;
		count += join_opts.mid.count;
    }
    // remove last mid
	count -= join_opts.mid.count;

    // allocate count
    u8* buffer = push_array(arena, u8, count + 1);
    u8* ptr = buffer;

    // write pre
    memcpy(ptr, join_opts.pre.data, join_opts.pre.count);
    ptr += join_opts.pre.count;

    bool is_mid = false;
    for(String8Node* node = str_sentinel->next; node != str_sentinel; node = node->next){
        // write mid
        if(is_mid && join_opts.mid.count){
            memcpy(ptr, join_opts.mid.data, join_opts.mid.count);
            ptr += join_opts.mid.count;
        }

        // write node string
        String8 node_string = node->string;
        memcpy(ptr, node_string.data, node_string.count);
        ptr += node_string.count;

        is_mid = true;
    }

    // write post
    memcpy(ptr, join_opts.post.data, join_opts.post.count);
    ptr += join_opts.post.count;

    // write zero, why? Because OS functions expect null terminated strings.
    *ptr = 0;

    String8 result = {buffer, count};
    return(result);
}

static String8
str8_path_append(Arena* arena, String8 path, String8 value){
    ScratchArena scratch = begin_scratch();

    String8Node* parts = str8_split(scratch.arena, path, '/');
    str8_list_push_back(scratch.arena, parts, value);
    String8Join join = {0};
    join.mid = str8_literal("/");
    String8 result = str8_join(arena, parts, join);
    end_scratch(scratch);

    return(result);
}

static String8
str8_path_pop(Arena* arena, String8 path, char slash){
    ScratchArena scratch = begin_scratch();

    String8Node* parts = str8_split(scratch.arena, path, slash);
    dll_pop_back(parts);

    String8Join join = {0};
    join.mid = str8_literal("/");
    String8 result = str8_join(arena, parts, join);

    end_scratch(scratch);

    return(result);
}

/////////////////////////
// note: OPERATOR OVERLOADING
/////////////////////////
#if STANDARD_CPP
static bool operator==(String8 a, String8 b){
    bool result = str8_compare(a, b);
    return(result);
}

static bool operator==(String16 a, String16 b){
    if(a.size != b.size){
        return(false);
    }
    for(u64 i = 0; i < a.size; ++i){
        if(*a.str++ != *b.str++){
            return(false);
        }
    }
    return(true);
}

static bool operator!=(String8 a, String8 b){
    return(!(a == b));
}

static bool operator!=(String16 a, String16 b){
    return(!(a == b));
}
#endif

#endif //BASE_STRING_H

