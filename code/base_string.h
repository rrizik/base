#ifndef BASE_STRING_H
#define BASE_STRING_H

#include "base_types.h"
#include "base_linkedlist.h"
#include "base_memory.h"

// note: All functions must 0 terminal strings when making allocations
// todo: Test that all functions 0 terminal strings
// todo: Get rid of String16 completely and only use wchar_t
// todo: Get rid of String32 completely

///////////////////////////////
// note: String8
///////////////////////////////

typedef struct String8{
    u8* str;
    u64 size;
} String8;

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

#define str8(str, size) str8_((u8*)str, size)
#define str8_literal(str) str8_((u8*)str, (sizeof(str) - 1))
static String8 str8_(u8* str, u64 size){
    String8 result = {str, size};
    return(result);
}

#include <stdio.h>
#include <stdarg.h>
#define str8_format(arena, format, ...) str8_formatted(arena, format, ...)
static String8
str8_formatted(Arena* arena, const char* format, ...) {
    char buffer[4096] = {};
    va_list args;
    va_start(args, format);
    u64 count = (u64)vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    u8* str = (u8*)push_array(arena, u8, count);
    mem_copy(str, buffer, count);
    String8 result = str8(str, count);

    return(result);
}

// untested:
//static String8 str8_cstring(u8* cstr){
//    u8* ptr = cstr;
//    for(;*ptr != 0; ptr+=1);
//    String8 result = str8_range(cstr, ptr);
//    return(result);
//}
//
// untested:
static bool
str8_copy(String8* from, String8* to){
    if(from->size != to->size){
        return(false);
    }

    for(u32 i=0; i < from->size; ++i){
        u8* character = to->str + i;
        *character = *(from->str + i);
    }
    return(true);
}

// untested:
static String8
str8_range(u8* first, u8* opl){
    String8 result = {first, (u64)(opl - first)};
    return(result);
}

// untested:
#define str8_clamp_right(str, size) str8_slice_left(str, size)
static String8
str8_slice_right(String8 str, u64 size){
    u64 min = MIN(size, str.size);
    String8 result = {str.str, min};
    return(result);
}

// untested:
static String8
str8_split_left(String8 str, u64 idx){
    str.size = idx;
    return(str);
}

// untested:
static String8
str8_split_right(String8 str, u64 idx){
    str.str = str.str + idx;
    str.size = str.size - idx;
    return(str);
}

//untested:
static String8
str8_advance(String8 str, u64 count){
    if(str.size > 0){
        str.str = str.str + count;
        str.size -= count;
    }
    return(str);
}

//untested:
static void
str8_advance(String8* str, u64 count){
    str->str = str->str + count;
    str->size -= count;
}

// untested:
// CONSIDER: Maybe this should return u64 count, to tell you how many u8's it ate.
// CONSIDER: Maybe have other functions like this, like eat_digits, eat_number, eat_string, ...
static String8
str8_eat_whitespace(String8 str){
    while(str.size){
        if((str.str[0] != ' ') && (str.str[0] != '\t') && (str.str[0] != '\n') && (str.str[0] != '\r')){ return(str); }
        str = str8_advance(str, 1);
    }
    return(str);
}

static u64
str8_eat_whitespace(String8* str){
    u64 count = 0;
    while(str->size){
        if((str->str[0] != ' ') && (str->str[0] != '\t') && (str->str[0] != '\n') && (str->str[0] != '\r')){
            u32 a = 1;
            break;
        }
        str8_advance(str, 1);
        count++;
    }
    return(count);
}

// untested:
static u64
str8_char_idx_from_left(String8 str, u8 character){
    u64 idx = 0;
    while(str.size){
        if(str.str[0] == character){ break; }
        str = str8_advance(str, 1);
        idx++;
    }
    return(idx);
}

// untested:
static u64
str8_next_white_space(String8 str){
    u64 idx = 0;
    while(str.size){
        if(str.str[0] == ' ' || str.str[0] == '\t' || str.str[0] == '\n' || str.str[0] == '\r'){ break; }
        str = str8_advance(str, 1);
        idx++;
    }
    return(idx);
}

// untested:
#define str8_clamp_left(str, size) str8_slice_left(str, size)
static String8
str8_slice_left(String8 str, u64 size){
    u64 min = MIN(size, str.size);
    u64 skip_to = str.size - min;
    String8 result = {str.str + skip_to, min};
    return(result);
}

// untested:
//static String8
//str8_clamp_right(String8 str, u64 size){
//    u64 min = MIN(size, str.size);
//    u64 new_size = str.size - min;
//    String8 result = {str.str, new_size};
//    return(result);
//}

// untested:
//static String8
//str8_clamp_left(String8 str, u64 size){
//    u64 min = MIN(size, str.size);
//    u64 skip_to = str.size - min;
//    String8 result = {str.str + skip_to, min};
//    return(result);
//}

// untested:
static String8
str8_substr_left(String8 str, u64 start, u64 end){
    u64 clamped_right = MIN(end, str.size);
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
    u8* str = (u8*)push_array(arena, u8, (left.size + right.size));
    String8 result = {str, (left.size + right.size)};

    for(u32 i = 0; i < left.size; ++i){
        *str++ = *(left.str + i);
    }
    for(u32 i = 0; i < right.size; ++i){
        *str++ = *right.str++;
    }

    return(result);
}

static String8
str8_null_terminate(Arena* arena, String8 input){
    u8* str = push_array(arena, u8, input.size + 1);
    mem_copy(str, input.str, input.size + 1);

    //String8 result = {
    //    .str = str,
    //    .size = input.size + 1,
    //};
    String8 result = {0};
    result.str = str;
    result.size = input.size + 1;

    u8* last = str + input.size;
    *last = 0;

    return(result);
}

#define str8_cmp(left, right) str8_compare(left, right)
static bool
str8_compare(String8 left, String8 right){
    if(left.size != right.size){
        return(false);
    }

    for(u32 i=0; i < left.size; ++i){
        if(left.str[i] != right.str[i]){
            return(false);
        }
    }

    return(true);
}

// untested:
static bool
str8_is_slash(u8 c){
    return(c == '\\' || c == '/');
}

static bool
str8_is_digit(u8 c){
    bool result = ((c >= '0') && (c <= '9'));
    return(result);
}

// untested:
static bool
str8_starts_with(String8 source_string, String8 sub_string){
    for(u32 i=0; i < sub_string.size; ++i){
        if(source_string.str[i] != sub_string.str[i]){
            return(false);
        }
    }
    return(true);
}

// untested:
static bool
str8_contains(String8 source_string, String8 sub_string){
    u32 count = 0;
    for(u32 i=0; i < source_string.size; ++i){
        if(source_string.str[i] == sub_string.str[i]){
            count++;
        }
        else{
            count = 0;
        }

        if(count == sub_string.size){
            return(true);
        }
    }
    return(false);
}

// untested:
// I think we need to reset the str pointer here?
// otherwise we will be pointing to the end of the string
#define str_length(str) str_length_((char*)str)
static u32 str_length_(char* str){
    u32 count = 0;
    while(*str++){
        ++count;
    }
    return(count);
}

// untested:
// I think we need to reset the str pointer here?
// otherwise we will be pointing to the end of the string
static u64 str_length_(wchar* str){
    u32 count = 0;
    while(*str++){
        ++count;
    }
    return(count);
}

// untested:
static String8
push_string(Arena* arena, String8 value){
    u8* str = push_array(arena, u8, value.size + 1);
    mem_copy(str, value.str, value.size);
    String8 result = {str, value.size};
    return(result);
}

static void
str8_list_push_back(Arena* arena, String8Node* str8_sentinel, String8 string){
    String8Node* string_node = push_array(arena, String8Node, 1);
    string_node->str.str = push_array(arena, u8, string.size);
    mem_copy(string_node->str.str, string.str, string.size);
    string_node->str.size = string.size;
    dll_push_back(str8_sentinel, string_node);
}

static String8Node
str8_split(Arena* arena, String8 string, char byte){
    String8Node parts = {};
    parts.next = &parts;
    parts.prev = &parts;

    u8* ptr =   string.str;
    u8* first = string.str;
    u8* opl =   string.str + string.size;
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
    u64 size = 0;

    // calc total size
    size += join_opts.pre.size;
    size += join_opts.post.size;
    for(String8Node* node = str8_sentinel->next; node != str8_sentinel; node = node->next){
        size += node->str.size;
		size += join_opts.mid.size;
    }
	size -= join_opts.mid.size;

    // allocate size
    u8* str = push_array(arena, u8, size + 1);
    u8* ptr = str;

    // write pre
    mem_copy(ptr, join_opts.pre.str, join_opts.pre.size);
    ptr += join_opts.pre.size;

    bool is_mid = false;
    for(String8Node* node = str8_sentinel->next; node != str8_sentinel; node = node->next){
        // write mid
        if(is_mid && join_opts.mid.size){
            mem_copy(ptr, join_opts.mid.str, join_opts.mid.size);
            ptr += join_opts.mid.size;
        }

        // write node string
        String8 node_string = node->str;
        mem_copy(ptr, node_string.str, node_string.size);
        ptr += node_string.size;

        is_mid = true;
    }

    // write post
    mem_copy(ptr, join_opts.post.str, join_opts.post.size);
    ptr += join_opts.post.size;

    // write zero, why? Because OS functions expect null terminated strings.
    *ptr = 0;

    String8 result = {str, size};
    return(result);
}


// untested:
// todo: Maybe pass in String8Join?
static String8
str8_path_append(Arena* arena, String8 path, String8 value){
    ScratchArena scratch = begin_scratch(0);

    String8Node parts = str8_split(scratch.arena, path, '\\');
    str8_list_push_back(scratch.arena, &parts, value);
    String8Join join = {0};
    join.mid = str8_literal("\\");
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

#if STANDARD_CPP
//static bool operator==(const String8& a, const String8& b){
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

#define str16(str, size) str16_((u16*)str, size)
static String16 str16_(u16* str, u64 size){
    String16 result = {str, size};
    return(result);
}

#define str32(str, size) str32_((u32*)str, size)
static String32 str32_(u32* str, u32 size){
    String32 result = {str, size};
    return(result);
}

#endif //BASE_STRING_H
