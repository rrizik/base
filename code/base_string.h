
#ifndef BASE_STRING_H
#define BASE_STRING_H

#include "base_types.h"
#include "base_memory.h"

///////////////////////////////
// INCOMPLETE: NOTE: String8
///////////////////////////////
// Just write it as you need it.... bitch

typedef struct String8{
    u8* str;
    u64 size;
} String8;

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
static String8 str8_(u8* str, u32 size){
    String8 result = {str, size};
    return(result);
}

// UNTESTED:
static String8
str8_range(u8* start, u8* opl){
    String8 result = {start, (u64)(opl - start)};
    return(result);
}

// UNTESTED:
static String8 str8_cstring(u8* cstr){
    u8* ptr = cstr;
    for(;*ptr != 0; ptr+=1);
    String8 result = str8_range(cstr, ptr);
    return(result);
}

#define str16(str, size) str16_((u16*)str, size)
static String16 str16_(u16* str, u32 size){
    String16 result = {str, size};
    return(result);
}

#define str32(str, size) str32_((u32*)str, size)
static String32 str32_(u32* str, u32 size){
    String32 result = {str, size};
    return(result);
}

// UNTESTED:
static String8
str8_slice_right(String8 str, u64 size){
    u64 min = MIN(size, str.size);
    String8 result = {str.str, min};
    return(result);
}

// UNTESTED:
static String8
str8_slice_left(String8 str, u64 size){
    u64 min = MIN(size, str.size);
    u64 skip_to = str.size - min;
    String8 result = {str.str + skip_to, min};
    return(result);
}

// UNTESTED:
static String8
str8_clamp_right(String8 str, u64 size){
    u64 min = MIN(size, str.size);
    u64 new_size = str.size - min;
    String8 result = {str.str, new_size};
    return(result);
}

// UNTESTED:
static String8
str8_clamp_left(String8 str, u64 size){
    u64 min = MIN(size, str.size);
    u64 skip_to = str.size - min;
    String8 result = {str.str + skip_to, min};
    return(result);
}

// UNTESTED:
static String8
str8_substr_left(String8 str, u64 start, u64 end){
    u64 clamped_right = MIN(end, str.size);
    u64 clamped_left = MIN(start, clamped_right);
    String8 result = {str.str + clamped_left, clamped_right - clamped_left};
    return(result);
}

// UNTESTED:
static String8
str8_substr_left_right(String8 str, u64 start, u64 end){
    String8 result = str8_substr_left(str, start, start + end);
    return(result);
}

static String8
str8_concatenate(Arena* arena, String8 left, String8 right){
    u8* str = (u8*)push_array(arena, u8, (left.size + right.size));
    String8 result = {str, (left.size + right.size)};

    for(s32 i = 0; i < left.size; ++i){
        *str++ = *left.str++;
    }
    for(s32 i = 0; i < right.size; ++i){
        *str++ = *right.str++;
    }

    return(result);
}

// TODO: Decide on compare or equals
// UNTESTED:
#define str8_equals(left, right) str8_compare(left, right)
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

static bool
str8_starts_with(String8 source_string, String8 sub_string){
    for(u32 i=0; i < sub_string.size; ++i){
        if(source_string.str[i] != sub_string.str[i]){
            return(false);
        }
    }
    return(true);
}

// UNTESTED:
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

// UNTESTED:
#define str_length(str) str_length_((char*)str)
static u32 str_length_(char* str){
    u32 count = 0;
    while(*str++){
        ++count;
    }
    return(count);
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

#if STANDARD_CPP
static bool operator==(const String8& a, const String8& b){
    u8* a_string = (u8*)a.str;
    u8* b_string = (u8*)b.str;
    if(a.size != b.size){
        return(false);
    }
    for(u32 i = 0; i < a.size; ++i){
        if(*a_string++ != *b_string++){
            return(false);
        }
    }
    return(true);
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


#endif //BASE_STRING_H
