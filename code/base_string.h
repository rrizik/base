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
    u32 length;
} String8;

typedef struct String16{
    u16* str;
    u32 length;
} String16;

typedef struct String32{
    u32* str;
    u32 length;
} String32;

#define str8_literal(str) str8_create_((u8*)str, (sizeof(str) - 1))
#define str8(str, length) str8_create_((u8*)str, length)
static String8 str8_create_(u8* str, u32 length){
    String8 result = {str, length};
    return(result);
}

#define str16(str, length) str16_((u16*)str, length)
static String16 str16_(u16* str, u32 length){
    String16 result = {str, length};
    return(result);
}

#define str32(str, length) str32_((u32*)str, length)
static String32 str32_(u32* str, u32 length){
    String32 result = {str, length};
    return(result);
}

static String8
str8_concatenate(Arena* arena, String8 left, String8 right){
    u8* str = (u8*)push_size(arena, (left.length + right.length));
    String8 result = {str, (left.length + right.length)};

    for(s32 i = 0; i < left.length; ++i){
        *str++ = *left.str++;
    }
    for(s32 i = 0; i < right.length; ++i){
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

#endif
