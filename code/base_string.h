#ifndef BASE_STRING_H
#define BASE_STRING_H

#include "base_types.h"
#include "base_linkedlist.h"
#include "base_memory.h"

/* JB String stuff

to_lower_in_place :: (s: string);
to_lower_copy :: (s: string, allocator := Basic.temporary_allocator) -> string;

to_upper_in_place :: (s: string);
to_upper_copy :: (s: string, allocator := Basic.temporary_allocator) -> string;

slice :: inline (s: string, index: s64, count: s64) -> string;
join :: (inputs: .. string, separator := "", before_first := false, after_last := false, allocator: Allocator = .{}) -> string;


split_from_left  :: (s: string, byte: u8) -> (found: bool, left: string, right: string);
split_from_right :: (s: string, byte: u8) -> (found: bool, left: string, right: string);
split_from_left  :: (s: string, separator: string) -> (found: bool, left: string, right: string);
split_from_right :: (s: string, separator: string) -> (found: bool, left: string, right: string);
split_from_left_by_any  :: (s: string, bytes: string) -> (found: bool, left: string, right: string);
split_from_right_by_any :: (s: string, bytes: string) -> (found: bool, left: string, right: string);
split :: (s: string, separator: $T) -> (result: [] string);

stop_at_any :: (s: string, bytes: string) -> string;

eat_trailing_spaces :: (_s: string) -> string;


Path routines:


path_decomp :: (name: string) -> (path: string, basename: string, extension: string, basename_with_extension: string);
    Separate the file path 'name' into its compontents.
path_filename :: (path: string) -> string;
    path_filename("/home/wizard/rezrov.foo") returns "rezrov.foo".
path_extension :: (path: string) -> (extension: string, found_extension: bool);
path_strip_filename :: (path: string) -> string;
path_strip_extension :: (path: string) -> string;
path_overwrite_separators :: (path: string, separator: u8 = PATH_SEPARATOR);
    Modify 'path' in-place to convert one kind of directory separator to another.
    Any occurrence of #char "\" or #char "/" in 'path' will be replaced by 'separator'.

is_absolute_path :: (path: string) -> bool;

*/
// todo: Test that all functions 0 terminal strings
// todo: Get rid of String16 completely and only use wchar_t
// todo: Get rid of String32 completely
// todo:: make all paths coming from OS forward slashed
// todo:: Write a function to_c_string() or equivalent that zero terminates a String8 when I need it. Also is_zero_terminated() function to check
// todo:: introduce union with data/count, str/size, str/length
// todo:: make a str8_add()/array_add() helper function
// consider: this should work only on dynamic arrays, maybe?

///////////////////////////////
// note: String8
///////////////////////////////

// note: All functions must 0 terminal strings when making allocations
typedef union String8{
    struct{ u8* str; u64 size; };
    struct{ u8* data; u64 count; };
} String8;
//typedef struct String8{
//    u8* str;
//    u64 size;
//} String8;

typedef struct String8Node{
    String8Node* next;
    String8Node* prev;
    String8 str;
} String8List;

static String8Node*
push_str8_node(Arena* arena){
    String8Node* result = push_struct(arena, String8Node);
    result->next = result;
    result->prev = result;
    return(result);
}

typedef struct String8Join{
    String8 pre;
    String8 mid;
    String8 post;
} String8Join;

typedef struct String16{
    u16* str;
    u64 size;
} String16;

typedef struct String32{
    u32* str;
    u64 size;
} String32;


///////////////////////////////
// NOTE: String8 Basic
///////////////////////////////

// Allocates memory.
// to_c_string :: (s: string) -> *u8 {
//
// string_to_float:: (_arg: string) -> result: $T, success: bool, remainder: string {
// string_to_int :: (t: string, base := 10, $T := int) -> result: T, success: bool, remainder: string {

#define str8(byte_buffer, count) str8_((u8*)byte_buffer, count)
#define str8_literal(byte_buffer) str8_((u8*)byte_buffer, (sizeof(byte_buffer) - 1))
static String8 str8_(u8* byte_buffer, u64 count){
    String8 result = {byte_buffer, count};
    return(result);
}

#include <stdio.h>
#include <stdarg.h>
#define str8_format(arena, format) str8_formatted(arena, format, ...)
static String8
str8_formatted(Arena* arena, const char* format, ...) {
    char buffer[4096] = {};
    va_list args;
    va_start(args, format);
    u64 count = (u64)vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    u8* byte_buffer = (u8*)push_array(arena, u8, count);
    memory_copy(byte_buffer, buffer, (u32)count);
    String8 result = str8(byte_buffer, count);

    return(result);
}

static bool
byte_is_slash(u8 c){
    return(c == '\\' || c == '/');
}

static bool
byte_is_digit(u8 c){
    bool result = ((c >= '0') && (c <= '9'));
    return(result);
}

static bool
byte_is_space(u8 c){
    if(c == ' ' || c == '\n' || c == '\t' || c == '\r'){
        return(true);
    }
    return(false);
}

static bool
byte_is_alpha(u8 c){
    if((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return(true);
    }
    return(false);
}

static bool
byte_is_alnum(u8 c){
    if(byte_is_alpha(c) || byte_is_digit(c)){
        return(true);
    }
    return(false);
}

static u8
byte_to_upper(u8 c){
    if(c >= 'a' && c <= 'z'){
        return((u8)(c + ('A' - 'a')));
    }
    return(c);
}

static u8
byte_to_lower(u8 c){
    if(c >= 'A' && c <= 'Z'){
        return((u8)(c + 'a' - 'A'));
    }
    return(c);
}

static bool
byte_is_upper(u8 c){
    if(c >= 'A' && c <= 'Z'){
        return(true);
    }
    return(false);
}

static bool
byte_is_lower(u8 c){
    if(c >= 'a' && c <= 'z'){
        return(true);
    }
    return(false);
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

static String8
str8_advance(String8 str, u64 count){
    if(str.count > count){
        str.str = str.str + count;
        str.count -= count;
    }
    return(str);
}

static void
str8_advance(String8* str, u64 count){
    str->str = str->str + count;
    str->count -= count;
}

// CONSIDER: Maybe have other functions like this, like eat_digits, eat_number, eat_string, ...
static String8
str8_eat_spaces(String8 str){
    while(str.count){
        if((str.str[0] != ' ') && (str.str[0] != '\t') && (str.str[0] != '\n') && (str.str[0] != '\r') && (str.str[0] != '\v') && (str.str[0] != '\f')){
            break;
        }
        str = str8_advance(str, 1);
    }
    return(str);
}

static u64
str8_eat_spaces(String8* str){
    u64 count = 0;
    while(str->count){
        if((str->str[0] != ' ') && (str->str[0] != '\t') && (str->str[0] != '\n') && (str->str[0] != '\r') && (str->str[0] != '\v') && (str->str[0] != '\f')){
            break;
        }
        str8_advance(str, 1);
        count++;
    }
    return(count);
}

static String8
str8_eat_word(String8* string){
    String8 result = {0};
    str8_eat_spaces(string);

    u32 count = 0;
    String8* view = string;
    while(view->count){
        if(byte_is_space(*view->data)){
            break;
        }
        str8_advance(view, 1);
        ++count;
    }

    result = {string->data, count};
    return(result);
}

//static String8
//str8_eat_word(String8* string){
//    String8 result = {0};
//    str8_eat_spaces(string);
//
//    u8* ptr = string->str;
//    u32 count = 0;
//    if(string->count){
//        while(!byte_is_space(*ptr) && *ptr != '\0'){
//            count++;
//            ptr++;
//            if(count >= string->count){
//                result = {string->str, count};
//                str8_advance(string, count);
//                return(result);
//            }
//        }
//
//        result = {string->str, count};
//        str8_advance(string, count);
//    }
//
//    return(result);
//}

static String8
str8_eat_line(String8* string){
    String8 result = {0};

    u32 count = 0;
    String8* view = string;
    while(view->count){
        if(*view->data == '\n'){
            str8_advance(view, 1);
            ++count;
            break;
        }
        str8_advance(view, 1);
        ++count;
    }

    result = {string->data, count};
    return(result);
}

//static String8
//str8_next_line(String8* string){
//    String8 result = {0};
//    u8* ptr = string->str;
//
//    u32 count = 0;
//    while(*ptr != '\n' && *ptr != '\0'){
//        count++;
//        ptr++;
//        if(count >= string->count){
//            result = {string->str, count};
//            str8_advance(string, count);
//            return(result);
//        }
//    }
//    // note: include newline in count
//    if(*ptr == '\n'){
//        ++ptr; // consume newline char
//        ++count; // consume newline char
//    }
//
//    result = {string->str, count};
//    str8_advance(string, count);
//
//    return(result);
//}

// consider: str8_trim_both_sides_at_the_same_time()?
static bool
str8_trim_left(String8* string, u64 count){
    if(string->count > count){
        string->data = string->data + count;
        string->count -= count;
        return(true);
    }
    return(false);
}

static bool
str8_trim_right(String8* string, u64 count){
    if(string->count > count){
        string->count -= count;
        return(true);
    }
    return(false);
}

///////////////////////////////
// NOTE: String8 Non-Basic
///////////////////////////////

static bool
str8_compare(String8 left, String8 right){
    if(left.count != right.count){
        return(false);
    }

    for(u32 i=0; i < left.count; ++i){
        if(left.str[i] != right.str[i]){
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

    for(u32 i=0; i < left.count; ++i){
        if(byte_to_lower(left.str[i]) != byte_to_lower(right.str[i])){
            return(false);
        }
    }

    return(true);
}

static bool
str8_contains(String8 string, String8 sub_string){
    u32 count = 0;
    for(u32 i=0; i < string.count; ++i){
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
str8_contains_byte(String8 string, u8 c){
    for(u32 i=0; i < string.count; ++i){
        if(string.data[i] == c){
            return(true);
        }
    }
    return(false);
}

static bool
str8_contains_nocase(String8 string, String8 sub_string){
    u32 count = 0;
    for(u32 i=0; i < string.count; ++i){
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
    for(u32 i=0; i < sub_string.count; ++i){
        if(string.data[i] != sub_string.data[i]){
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

    u64 idx = string.count;
    for(u64 i=sub_string.count; i <= sub_string.count; --i){
        if(string.data[idx] != sub_string.data[i]){
            return(false);
        }
        --idx;
    }
    return(true);
}

static bool
str8_starts_with_nocase(String8 string, String8 sub_string){
    if(sub_string.count > string.count){
        return(false);
    }
    for(u32 i=0; i < sub_string.count; ++i){
        if(byte_to_lower(string.data[i]) != byte_to_lower(sub_string.data[i])){
            return(false);
        }
    }
    return(true);
}

static bool
str8_ends_with_nocase(String8 string, String8 sub_string){
    if(sub_string.count > string.count){
        return(false);
    }

    u64 idx = string.count;
    for(u64 i=sub_string.count; i <= sub_string.count; --i){
        if(byte_to_lower(string.data[idx]) != byte_to_lower(sub_string.data[i])){
            return(false);
        }
        --idx;
    }
    return(true);
}

static u64
str8_replace_chars(String8* string, u8 c_in, u8 c_out){
    u64 count = 0;
    for(s32 i=0; i < string->count; ++i){
        if(string->data[i] == c_in){
            string->data[i] = c_out;
            ++count;
        }
    }
    return(count);
}

static u64
byte_index_from_left(String8 string, u8 c){
    u64 index = 0;
    while(string.count){
        if(string.data[0] == c){ break; }
        string = str8_advance(string, 1);
        index++;
    }
    return(index);
}

static u64
str8_index_from_left(String8 string, String8 sub_string){
    if(string.count < sub_string.count){
        return(0);
    }

    bool first = true;
    u32 index = 0;
    u32 count = 0;
    for(u32 i=0; i < string.count; ++i){
        if(string.data[i] == sub_string.data[count]){
            if(first){
                index = i;
                first = false;
            }
            count++;
        }
        else{
            first = true;
            index = 0;
            count = 0;
        }

        if(count == sub_string.count){
            return(index);
        }
    }
    return(index);
}

static u64
byte_index_from_right(String8 string, u8 c){
    u64 index = 0;
    while(string.count){
        if(string.data[string.count - 1 - index] == c){ break; }
        str8_trim_right(&string, 1);
        index++;
    }
    return(index);
}

static u64
str8_index_from_right(String8 string, String8 sub_string){
    if(string.count < sub_string.count){
        return(0);
    }

    bool first = true;
    u32 index = 0;
    u32 count = 0;
    for(u32 i=0; i < string.count; ++i){
        if(string.data[string.count - 1 - i] == sub_string.data[sub_string.count - 1 - count]){
            if(first){
                index = i;
                first = false;
            }
            count++;
        }
        else{
            first = true;
            index = 0;
            count = 0;
        }

        if(count == sub_string.count){
            return(index);
        }
    }
    return(index);
}





















//static String8 str8_cstring(u8* cstr){
//    u8* ptr = cstr;
//    for(;*ptr != 0; ptr+=1);
//    String8 result = str8_range(cstr, ptr);
//    return(result);
//}
//
static bool
str8_copy(String8* from, String8* to){
    if(from->count != to->count){
        return(false);
    }

    for(u32 i=0; i < from->count; ++i){
        u8* character = to->str + i;
        *character = *(from->str + i);
    }
    return(true);
}

static String8
str8_range(u8* first, u8* opl){
    String8 result = {first, (u64)(opl - first)};
    return(result);
}

#define str8_clamp_right(str, count) str8_slice_left(str, count)
static String8
str8_slice_right(String8 str, u64 count){
    u64 min = MIN(count, str.count);
    String8 result = {str.str, min};
    return(result);
}

// untested:
static String8
str8_split_left(String8 str, u64 idx){
    str.count = idx;
    return(str);
}

// untested:
static String8
str8_split_right(String8 str, u64 idx){
    str.str = str.str + idx;
    str.count = str.count - idx;
    return(str);
}

// untested:
static u64
str8_next_space_idx(String8 str){
    u64 idx = 0;
    while(str.count){
        if(str.str[0] == ' ' || str.str[0] == '\t' || str.str[0] == '\n' || str.str[0] == '\r'){ break; }
        str = str8_advance(str, 1);
        idx++;
    }
    return(idx);
}

// untested:
#define str8_clamp_left(str, count) str8_slice_left(str, count)
static String8
str8_slice_left(String8 str, u64 count){
    u64 min = MIN(count, str.count);
    u64 skip_to = str.count - min;
    String8 result = {str.str + skip_to, min};
    return(result);
}

// untested:
//static String8
//str8_clamp_right(String8 str, u64 count){
//    u64 min = MIN(count, str.count);
//    u64 new_count = str.count - min;
//    String8 result = {str.str, new_count};
//    return(result);
//}

// untested:
//static String8
//str8_clamp_left(String8 str, u64 count){
//    u64 min = MIN(count, str.count);
//    u64 skip_to = str.count - min;
//    String8 result = {str.str + skip_to, min};
//    return(result);
//}

// untested:
static String8
str8_substr_left(String8 str, u64 start, u64 end){
    u64 clamped_right = MIN(end, str.count);
    u64 clamped_left = MIN(start, clamped_right);
    String8 result = {str.str + clamped_left, clamped_right - clamped_left};
    return(result);
}

// untested:
static String8
str8_substr_left_right(String8 str, u64 start, u64 end){
    String8 result = str8_substr_left(str, start, start + end);
    return(result);
}

static String8
str8_concatenate(Arena* arena, String8 left, String8 right){
    u8* str = (u8*)push_array(arena, u8, (left.count + right.count));
    String8 result = {str, (left.count + right.count)};

    for(u32 i = 0; i < left.count; ++i){
        *str++ = *(left.str + i);
    }
    for(u32 i = 0; i < right.count; ++i){
        *str++ = *right.str++;
    }

    return(result);
}

static String8
str8_null_terminate(Arena* arena, String8 input){
    u8* str = push_array(arena, u8, input.count + 1);
    memory_copy(str, input.str, input.count + 1);

    String8 result = {0};
    result.str = str;
    result.count = input.count + 1;

    u8* last = str + input.count;
    *last = 0;

    return(result);
}

// untested:
// I think we need to reset the str pointer here?
// otherwise we will be pointing to the end of the string
static u32 char_length(char* str){
    u32 count = 0;
    while(*str++){
        ++count;
    }
    str = str - count;
    return(count);
}

static u32 char_copy(char* dst, char* src){
    u32 count = 0;
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

// untested:
// I think we need to reset the str pointer here?
// otherwise we will be pointing to the end of the string
static u64 wchar_length(wchar* str){
    u32 count = 0;
    while(*str++){
        ++count;
    }
    str = str - count;
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
// untested:
static String8
push_str8(Arena* arena, String8 value){
    u8* str = push_array(arena, u8, value.count + 1);
    memory_copy(str, value.str, value.count);
    String8 result = {str, value.count};
    return(result);
}

static void
str8_list_push_back(Arena* arena, String8Node* str8_sentinel, String8 string){
    String8Node* string_node = push_array(arena, String8Node, 1);
    string_node->str.str = push_array(arena, u8, string.count);
    memory_copy(string_node->str.str, string.data, string.count);
    string_node->str.count = string.count;
    dll_push_back(str8_sentinel, string_node);
}

static String8Node
str8_split(Arena* arena, String8 string, char byte){
    String8Node parts = {};
    parts.next = &parts;
    parts.prev = &parts;

    u8* ptr =   string.data;
    u8* first = string.data;
    u8* opl =   string.data + string.count;
    for(; ptr < opl; ptr += 1){
        bool is_split_byte = false;
        if(*ptr == byte){
            is_split_byte = true;
        }

        if(is_split_byte){
            if(first < ptr){
                str8_list_push_back(arena, &parts, str8_range(first, ptr));
            }
            first = ptr + 1;
        }
    }

    if(first < ptr){
        str8_list_push_back(arena, &parts, str8_range(first, ptr));
    }

    return(parts);
}

static String8
str8_join(Arena* arena, String8Node* str8_sentinel, String8Join join_opts){
    u64 count = 0;

    // calc total count
    count += join_opts.pre.count;
    count += join_opts.post.count;
    for(String8Node* node = str8_sentinel->next; node != str8_sentinel; node = node->next){
        count += node->str.count;
		count += join_opts.mid.count;
    }
	count -= join_opts.mid.count;

    // allocate count
    u8* str = push_array(arena, u8, count + 1);
    u8* ptr = str;

    // write pre
    memory_copy(ptr, join_opts.pre.str, join_opts.pre.count);
    ptr += join_opts.pre.count;

    bool is_mid = false;
    for(String8Node* node = str8_sentinel->next; node != str8_sentinel; node = node->next){
        // write mid
        if(is_mid && join_opts.mid.count){
            memory_copy(ptr, join_opts.mid.str, join_opts.mid.count);
            ptr += join_opts.mid.count;
        }

        // write node string
        String8 node_string = node->str;
        memory_copy(ptr, node_string.data, node_string.count);
        ptr += node_string.count;

        is_mid = true;
    }

    // write post
    memory_copy(ptr, join_opts.post.str, join_opts.post.count);
    ptr += join_opts.post.count;

    // write zero, why? Because OS functions expect null terminated strings.
    *ptr = 0;

    String8 result = {str, count};
    return(result);
}


// untested:
// TODO: IMPORTANT: ADD PARAMETER SLASH CHAR TO SPLIT ON '/' AND '\\'
static String8
str8_path_append(Arena* arena, String8 path, String8 value){
    ScratchArena scratch = begin_scratch();

    String8Node parts = str8_split(scratch.arena, path, '/');
    str8_list_push_back(scratch.arena, &parts, value);
    String8Join join = {0};
    join.mid = str8_literal("/");
    String8 result = str8_join(arena, &parts, join);
    end_scratch(scratch);

    return(result);
}

static String8
str8_path_pop(Arena* arena, String8 path, char slash){
    ScratchArena scratch = begin_scratch();

    String8Node parts = str8_split(scratch.arena, path, slash);
    dll_pop_back(&parts);

    String8Join join = {0};
    join.mid = str8_literal("/");
    String8 result = str8_join(arena, &parts, join);

    end_scratch(scratch);

    return(result);
}

// copy str8
// str_in:
// char_in:
// char_replace:
// str_index:
// str_upper:
// str_is_upper:
// str_lower:
// str_is_lower:
// str_strop:
// str_split:
// str_ends_with:
// str_range:
// append:
// str8_chop_last_period()
// str8_skip_last_period()
// str8_chop_last_lash()
// str8_skip_last_lash()
//
//
//

/////////////////////////
// OPERATOR OVERLOADING
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
    for(u32 i = 0; i < a.size; ++i){
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

#define str16(str, count) str16_((u16*)str, count)
static String16 str16_(u16* str, u64 count){
    String16 result = {str, count};
    return(result);
}

#define str32(str, count) str32_((u32*)str, count)
static String32 str32_(u32* str, u32 count){
    String32 result = {str, count};
    return(result);
}

#endif //BASE_STRING_H
