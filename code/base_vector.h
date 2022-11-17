#ifndef BASE_VECTOR_H
#define BASE_VECTOR_H

///////////////////////////////
// NOTE: Compound Types
///////////////////////////////
//TODO: inline a [] operator for all compoint types

typedef union v2{
    struct{ f32 x; f32 y; };
    struct{ f32 w; f32 h; };
    f32 e[2];
} v2;

typedef union v3{
    struct{ f32 x; f32 y; f32 z; };
    f32 e[3];
} v3;

typedef union v4{
    struct{ f32 x; f32 y; f32 z; f32 w; };
    f32 e[4];
} v4;

typedef union v2s32{
    struct{ s32 x; s32 y; };
    struct{ s32 w; s32 h; };
    s32 e[2];
} v2s32;

//UNTESTED:
typedef union RGBA{
    struct{ f32 r; f32 g; f32 b; f32 a; };
    f32 e[4];
} RGBA;

///////////////////////////////
// NOTE: Compound Types Operators
///////////////////////////////

#if STANDARD_CPP
static v2s32 operator+(const v2s32& a, const v2s32& b){
    v2s32 result = {a.x + b.x, a.y + b.y};
    return(result);
}

static v2 operator+(const v2& a, const v2& b){
    v2 result = {a.x + b.x, a.y + b.y};
    return(result);
}

static v3 operator+(const v3& a, const v3& b){
    v3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return(result);
}

static v4 operator+(const v4& a, const v4& b){
    v4 result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return(result);
}

static RGBA operator+(const RGBA& a, const RGBA& b){
    RGBA result = {a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a};
    return(result);
}

static v2s32 operator-(const v2s32& a, const v2s32& b){
    v2s32 result = {a.x - b.x, a.y - b.y};
    return(result);
}

static v2 operator-(const v2& a, const v2& b){
    v2 result = {a.x - b.x, a.y - b.y};
    return(result);
}

static v3 operator-(const v3& a, const v3& b){
    v3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return(result);
}

static v4 operator-(const v4& a, const v4& b){
    v4 result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return(result);
}

static RGBA operator-(const RGBA& a, const RGBA& b){
    RGBA result = {a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a};
    return(result);
}

static v2s32 operator*(const v2s32& a, const s32 b){
    v2s32 result = {a.x * b, a.y * b};
    return(result);
}

static v3 operator*(const v3& b, const f32& a){
    v3 result = {b.x * a, b.y * a, b.z * a};
    return(result);
}

static v4 operator*(const v4& b, const f32& a){
    v4 result = {b.x * a, b.y * a, b.z * a, b.w * a};
    return(result);
}

static v2s32 operator*(const s32& a, const v2s32& b){
    v2s32 result = {b.x * a, b.y * a};
    return(result);
}

static v2 operator*(const v2& b, const f32& a){
    v2 result = {b.x * a, b.y * a};
    return(result);
}

static v2 operator*(const f32& a, const v2& b){
    v2 result = {b.x * a, b.y * a};
    return(result);
}

static v3 operator*(const f32& a, const v3& b){
    v3 result = {b.x * a, b.y * a, b.z * a};
    return(result);
}

static v4 operator*(const f32& a, const v4& b){
    v4 result = {b.x * a, b.y * a, b.z * a, b.w * a};
    return(result);
}

static bool operator==(const v2s32& a, const v2s32& b){
    return((a.x == b.x) && (a.y == b.y));
}

static bool operator==(const v2& a, const v2& b){
    return((a.x == b.x) && (a.y == b.y));
}

static bool operator==(const v3& a, const v3& b){
    return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

static bool operator==(const v4& a, const v4& b){
    return((a.x == b.x) && (a.y == b.y && (a.z == b.z) && (a.w == b.w)));
}

static bool operator==(const RGBA& a, const RGBA& b){
    return((a.r == b.r) && (a.g == b.g && (a.b == b.b) && (a.a == b.a)));
}

static bool operator!=(const v2s32& a, const v2s32& b){
    return(!(a == b));
}

static bool operator!=(const v2& a, const v2& b){
    return(!(a == b));
}

static bool operator!=(const v3& a, const v3& b){
    return(!(a == b));
}

static bool operator!=(const v4& a, const v4& b){
    return(!(a == b));
}

static bool operator!=(const RGBA& a, const RGBA& b){
    return(!(a == b));
}
#endif

///////////////////////////////
// NOTE: Vector Math
///////////////////////////////

// Vector2
static v2
round_v2(v2 value){
    v2 result = {};
    result.x = (f32)((s32)(value.x + 0.5f));
    result.y = (f32)((s32)(value.y + 0.5f));
    return(result);
}

static v2s32
round_v2_v2s32(v2 value){
    v2s32 result = {};
    result.x = ((s32)(value.x + 0.5f));
    result.y = ((s32)(value.y + 0.5f));
    return(result);
}

static f32
inner_product_v2(v2 a, v2 b){
    return((a.x * b.x) + (a.y * b.y));
}

static bool
is_perpendicular_v2(v2 a, v2 b){
    return(inner_product_v2(a, b) == 0);
}

#define right_direction_v2(a, b) same_direction_v2(a, b)
static bool same_direction_v2(v2 a, v2 b){
    return(inner_product_v2(a, b) > 0);
}

#define left_direction_v2(a, b) opposite_direction_v2(a, b)
static bool opposite_direction_v2(v2 a, v2 b){
    return(inner_product_v2(a, b) < 0);
}

static f32
magnitude_sqrt_v2(v2 a){
    return(inner_product_v2(a, a));
}

static f32
magnitude_v2(v2 a){
    return(sqrtf(inner_product_v2(a, a)));
}

static f32 distance_v2(v2 a, v2 b){
    f32 result = magnitude_v2(a - b);
    return(result);
}

static v2
normalized_v2(v2 a){
    v2 result = a * (1.0f / magnitude_v2(a));
    return(result);
}

static v2
direction_v2(v2 a, v2 b){
    v2 result = normalized_v2(b - a);
    return(result);
}

static f32
angle_v2(v2 a, v2 b){
    f32 magnitude = sqrtf(magnitude_sqrt_v2(a) * magnitude_sqrt_v2(b));
    f32 inner = inner_product_v2(a, b);
    f32 result = acosf(inner / magnitude);
    return(result);
}

static v2
project_v2(v2 a, v2 b){
    f32 inner =  inner_product_v2(a, b);
    f32 mag_sqrt = magnitude_sqrt_v2(b);
    v2 result = b * (inner / mag_sqrt);
    return(result);
}

static v2
perpendicular_v2(v2 a, v2 b){
    v2 result = a - project_v2(a, b);
    return(result);
}

static v2
reflection_v2(v2 a, v2 normal){
    f32 inner = inner_product_v2(a, normal);
    v2 result = a - (2.0f * (inner * normal));
    return(result);
}

// Vector3
static f32
inner_product_v3(v3 a, v3 b){
    return((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

static bool
is_perpendicular_v3(v3 a, v3 b){
    return(inner_product_v3(a, b) == 0);
}

#define right_direction_v3(a, b) same_direction_v3(a, b)
static bool same_direction_v3(v3 a, v3 b){
    return(inner_product_v3(a, b) > 0);
}

#define left_direction_v3(a, b) opposite_direction_v3(a, b)
static bool opposite_direction_v3(v3 a, v3 b){
    return(inner_product_v3(a, b) < 0);
}

static f32
magnitude_sqrt_v3(v3 a){
    f32 result = inner_product_v3(a, a);
    return(result);
}

static f32
magnitude_v3(v3 a){
    f32 result = sqrtf(inner_product_v3(a, a));
    return(result);
}

static f32
distance_v3(v3 a, v3 b){
    f32 result = magnitude_v3(a - b);
    return(result);
}

static v3
normalized_v3(v3 a){
    v3 result = a * (1.0f / magnitude_v3(a));
    return(result);
}

static v3
direction_v3(v3 a, v3 b){
    v3 result = normalized_v3(b - a);
    return(result);
}

static v3 // NOTE: produces a vector perpendicular to both vectors
cross_product_v3(v3 a, v3 b){
    v3 result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return(result);
}

static f32
angle_v3(v3 a, v3 b){
    f32 magnitude = sqrtf(magnitude_sqrt_v3(a) * magnitude_sqrt_v3(b));
    f32 inner = inner_product_v3(a, b);
    f32 result = acosf(inner / magnitude);
    return(result);
}

static v3
project_v3(v3 a, v3 b){
    f32 inner =  inner_product_v3(a, b);
    f32 mag_sqrt = magnitude_sqrt_v3(b);
    v3 result = b * (inner / mag_sqrt);
    return(result);
}

static v3
perpendicular_v3(v3 a, v3 b){
    v3 result = a - project_v3(a, b);
    return(result);
}

static v3
reflection_v3(v3 a, v3 normal){
    f32 inner = inner_product_v3(a, normal);
    return(a - ((2.0f * inner) * normal));
}

#endif
