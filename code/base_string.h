#if !defined(BASE_STRING_H)
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

#define str8_literal(str) str8_create_((u8*)str, (sizeof(str) - 1))
#define str8(str, size) str8_create_((u8*)str, size)
static String8 str8_create_(u8* str, u32 size){
    String8 result = {str, size};
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
// str_starts_with:
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


#endif
