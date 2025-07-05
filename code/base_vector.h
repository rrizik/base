#ifndef BASE_VECTOR_H
#define BASE_VECTOR_H

///////////////////////////////
// NOTE: Compound Types
///////////////////////////////
//TODO: inline a [] operator for all compoint types
//TODO: +=
//todo:: IMPORTANT: maybe change all the vector stuff to capitalization V2 or Vector2/Vector3

typedef union v2{
    struct{ f32 x; f32 y; };
    struct{ f32 w; f32 h; };
    f32 e[2];
} v2;

typedef union v3{
    struct{ f32 x; f32 y; f32 z; };
    struct{ f32 w; f32 h; f32 l; };
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
    union{
        v3 rgb;
    };
    f32 e[4];
} RGBA;

///////////////////////////////
// NOTE: Compound Types Constructors
///////////////////////////////

static v2 make_v2(f32 x, f32 y){
    v2 result = {{x, y}};
    return(result);
}

static v2s32 make_v2s32(s32 x, s32 y){
    v2s32 result = {x, y};
    return(result);
}

static v3 make_v3(f32 x, f32 y, f32 z){
    v3 result = {x, y, z};
    return(result);
}

static v4 make_v4(f32 x, f32 y, f32 z, f32 w){
    v4 result = {x, y, z, w};
    return(result);
}

static RGBA make_RGBA(f32 r, f32 g, f32 b, f32 a){
    RGBA result = {r, g, b, a};
    return(result);
}

static v2 v2_from_v3(v3 v){
    v2 result = {v.x, v.y};
    return(result);
}

static v2 v2_from_v2s32(v2s32 v){
    v2 result = {(f32)v.x, (f32)v.y};
    return(result);
}


///////////////////////////////
// NOTE: Compound Types Operators
///////////////////////////////

#if STANDARD_CPP
static v2s32 operator+(v2s32 a, v2s32 b){
    v2s32 result = {a.x + b.x, a.y + b.y};
    return(result);
}

static v2 operator+(v2 a, v2 b){
    v2 result = {a.x + b.x, a.y + b.y};
    return(result);
}

static v3 operator+(v3 a, v3 b){
    v3 result = {a.x + b.x, a.y + b.y, a.z + b.z};
    return(result);
}

static v4 operator+(v4 a, v4 b){
    v4 result = {a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w};
    return(result);
}

static RGBA operator+(RGBA a, RGBA b){
    RGBA result = {a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a};
    return(result);
}

static v2s32 operator-(v2s32 a){
    v2s32 result = {-a.x, -a.y};
    return(result);
}

static v2s32 operator-(v2s32 a, v2s32 b){
    v2s32 result = {a.x - b.x, a.y - b.y};
    return(result);
}

static v2 operator-(v2 a){
    v2 result = {-a.x, -a.y};
    return(result);
}

static v2 operator-(v2 a, v2 b){
    v2 result = {a.x - b.x, a.y - b.y};
    return(result);
}

static v3 operator-(v3 a, v3 b){
    v3 result = {a.x - b.x, a.y - b.y, a.z - b.z};
    return(result);
}

static v4 operator-(v4 a, v4 b){
    v4 result = {a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w};
    return(result);
}

static RGBA operator-(RGBA a, RGBA b){
    RGBA result = {a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a};
    return(result);
}

static v2s32 operator*(v2s32 a, s32 b){
    v2s32 result = {a.x * b, a.y * b};
    return(result);
}

static v3 operator*(v3 b, f32 a){
    v3 result = {b.x * a, b.y * a, b.z * a};
    return(result);
}

static v3 operator*=(v3 &result, f32 a){
    result = {result.x * a, result.y * a, result.z * a};
    return(result);
}

static v4 operator*(v4 b, f32 a){
    v4 result = {b.x * a, b.y * a, b.z * a, b.w * a};
    return(result);
}

static v2s32 operator*(s32 a, v2s32 b){
    v2s32 result = {b.x * a, b.y * a};
    return(result);
}

static v2 operator*(v2 b, f32 a){
    v2 result = {b.x * a, b.y * a};
    return(result);
}

static v2 operator*(f32 a, v2 b){
    v2 result = {b.x * a, b.y * a};
    return(result);
}

static v3 operator*(f32 a, v3 b){
    v3 result = {b.x * a, b.y * a, b.z * a};
    return(result);
}

static v4 operator*(f32 a, v4 b){
    v4 result = {b.x * a, b.y * a, b.z * a, b.w * a};
    return(result);
}

static RGBA operator*(f32 a, RGBA b){
    RGBA result = {b.r * a, b.g * a, b.b * a, b.a * a};
    return(result);
}

static bool operator==(v2s32 a, v2s32 b){
    return((a.x == b.x) && (a.y == b.y));
}

static bool operator==(v2 a, v2 b){
    return((a.x == b.x) && (a.y == b.y));
}

static bool operator==(v3 a, v3 b){
    return((a.x == b.x) && (a.y == b.y) && (a.z == b.z));
}

static bool operator==(v4 a, v4 b){
    return((a.x == b.x) && (a.y == b.y && (a.z == b.z) && (a.w == b.w)));
}

static bool operator==(RGBA a, RGBA b){
    return((a.r == b.r) && (a.g == b.g && (a.b == b.b) && (a.a == b.a)));
}

static bool operator!=(v2s32 a, v2s32 b){
    return(!(a == b));
}

static bool operator!=(v2 a, v2 b){
    return(!(a == b));
}

static bool operator!=(v3 a, v3 b){
    return(!(a == b));
}

static bool operator!=(v4 a, v4 b){
    return(!(a == b));
}

static bool operator!=(RGBA a, RGBA b){
    return(!(a == b));
}

#endif

// UNTESTED All of this stuff is untested
static v2s32 v2s32_add(v2s32 a, v2s32 b){
    v2s32 result = {
        a.x + b.x,
        a.y + b.y,
    };
    return(result);
}

static v2 v2_add(v2 a, v2 b){
    v2 result = {
        a.x + b.x,
        a.y + b.y,
    };
    return(result);
}

static v3 v3_add(v3 a, v3 b){
    v3 result = {
        a.x + b.x,
        a.y + b.y,
        a.y + b.y,
    };
    return(result);
}

static v4 v4_add(v4 a, v4 b){
    v4 result = {
        a.x + b.x,
        a.y + b.y,
        a.z + b.z,
        a.w + b.w,
    };
    return(result);
}

static v2s32 v2s32_sub(v2s32 a, v2s32 b){
    v2s32 result = {
        a.x - b.x,
        a.y - b.y,
    };
    return(result);
}

static v2 v2_sub(v2 a, v2 b){
    v2 result = {
        a.x - b.x,
        a.y - b.y,
    };
    return(result);
}

static v3 v3_sub(v3 a, v3 b){
    v3 result = {
        a.x - b.x,
        a.y - b.y,
        a.y - b.y,
    };
    return(result);
}

static v4 v4_sub(v4 a, v4 b){
    v4 result = {
        a.x - b.x,
        a.y - b.y,
        a.z - b.z,
        a.w - b.w,
    };
    return(result);
}

static v2s32 v2s32_mul(v2s32 a, s32 b){
    v2s32 result = {
        a.x * b,
        a.y * b,
    };
    return(result);
}

static v2 v2_scale(v2 a, f32 b){
    v2 result = {
        a.x * b,
        a.y * b,
    };
    return(result);
}

static v3 v3_mul(v3 a, f32 b){
    v3 result = {
        a.x * b,
        a.y * b,
        a.y * b,
    };
    return(result);
}

static v4 v4_mul(v4 a, f32 b){
    v4 result = {
        a.x * b,
        a.y * b,
        a.z * b,
        a.w * b,
    };
    return(result);
}

static bool v2s32_cmp(v2s32 a, v2s32 b){
    bool result = (
        a.x == b.x &&
        a.y == b.y
    );
    return(result);
}

static bool v2_cmp(v2 a, v2 b){
    bool result = (
        a.x == b.x &&
        a.y == b.y
    );
    return(result);
}

static bool v3_cmp(v3 a, v3 b){
    bool result = (
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z
    );
    return(result);
}

static bool v4_cmp(v4 a, v4 b){
    bool result = (
        a.x == b.x &&
        a.y == b.y &&
        a.z == b.z &&
        a.w == b.w
    );
    return(result);
}

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

// NOTE: Dot Product:
// If positive, the vectors are pointing in the same *general* direction
// If negative, the vectors point in opposing *general* directions
// If 0, the vectors are perpendicular important: You can use 0 to face things in the right direction (forward vector, right vector)

#define dot_v2(a, b) dot_product_v2(a, b)
static f32
dot_product_v2(v2 a, v2 b){
    return((a.x * b.x) + (a.y * b.y));
}

static bool
is_perpendicular_v2(v2 a, v2 b){
    return(dot_product_v2(a, b) == 0);
}

#define right_direction_v2(a, b) same_direction_v2(a, b)
static bool same_direction_v2(v2 a, v2 b){
    return(dot_product_v2(a, b) > 0);
}

#define left_direction_v2(a, b) opposite_direction_v2(a, b)
static bool opposite_direction_v2(v2 a, v2 b){
    return(dot_product_v2(a, b) < 0);
}

static f32
magnitude_squared_v2(v2 a){
    f32 result = dot_product_v2(a, a);
    return(result);
}

static f32
magnitude_v2(v2 a){
    f32 result = sqrtf(dot_product_v2(a, a));
    return(result);
}

static f32
distance_v2(v2 a, v2 b){
    f32 result = magnitude_v2(a - b);
    return(result);
}

static f32
distance_squared_v2(v2 a, v2 b){
    f32 result = magnitude_squared_v2(a - b);
    return(result);
}

static v2
normalize_v2(v2 a){
    v2 result = {0};
    f32 magnitude = magnitude_v2(a);
    if(magnitude != 0){
        result.x = a.x / magnitude;
        result.y = a.y / magnitude;
    }
    else{
        result = {0, 0};
    }
    return(result);
}

static v2
direction_v2(v2 from, v2 to){
    v2 result = normalize_v2(to - from);
    return(result);
}

static f32 // NOTE: produces a scalar
cross_product_v2(v2 a, v2 b){
    f32 result;
    result = (a.x * b.y) - (a.y * b.x);
    return(result);
}

static f32
angle_v2(v2 a, v2 b){
    f32 magnitude = sqrtf(magnitude_squared_v2(a) * magnitude_squared_v2(b));
    f32 dot = dot_product_v2(a, b);
    f32 result = acosf(dot / magnitude);
    return(result);
}

static v2
project_v2(v2 a, v2 b){
    f32 dot =  dot_product_v2(a, b);
    f32 mag_sqrt = magnitude_squared_v2(b);
    v2 result = v2_scale(b, (dot / mag_sqrt));
    return(result);
}

// UNTESTED
#define perp(a) perpendicular(a)
static v2
perpendicular(v2 a){
    v2 result = {-a.y, a.x};
    return(result);
}

static v2
perpendicular_v2(v2 a, v2 b){
    v2 result = a - project_v2(a, b);
    return(result);
}

static v2
reflection_v2(v2 a, v2 normal){
    f32 dot = dot_product_v2(a, normal);
    v2 result = a - (2.0f * (dot * normal));
    return(result);
}

// Vector3
#define dot_v3(a, b) dot_product_v3(a, b)
static f32
dot_product_v3(v3 a, v3 b){
    return((a.x * b.x) + (a.y * b.y) + (a.z * b.z));
}

static bool
is_perpendicular_v3(v3 a, v3 b){
    return(dot_product_v3(a, b) == 0);
}

#define right_direction_v3(a, b) same_direction_v3(a, b)
static bool same_direction_v3(v3 a, v3 b){
    return(dot_product_v3(a, b) > 0);
}

#define left_direction_v3(a, b) opposite_direction_v3(a, b)
static bool opposite_direction_v3(v3 a, v3 b){
    return(dot_product_v3(a, b) < 0);
}

static f32
magnitude_squared_v3(v3 a){
    f32 result = dot_product_v3(a, a);
    return(result);
}

static f32
magnitude_v3(v3 a){
    f32 result = sqrtf(dot_product_v3(a, a));
    return(result);
}

static f32
distance_v3(v3 a, v3 b){
    f32 result = magnitude_v3(a - b);
    return(result);
}

static v3
normalize_v3(v3 a){
    v3 result = a * (1.0f / magnitude_v3(a));
    return(result);
}

static v3
direction_v3(v3 a, v3 b){
    v3 result = normalize_v3(b - a);
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
    f32 magnitude = sqrtf(magnitude_squared_v3(a) * magnitude_squared_v3(b));
    f32 dot = dot_product_v3(a, b);
    f32 result = acosf(dot / magnitude);
    return(result);
}

static v3
project_v3(v3 a, v3 b){
    f32 dot =  dot_product_v3(a, b);
    f32 mag_sqrt = magnitude_squared_v3(b);
    v3 result = b * (dot / mag_sqrt);
    return(result);
}

static v3
perpendicular_v3(v3 a, v3 b){
    v3 result = a - project_v3(a, b);
    return(result);
}

static v3
reflection_v3(v3 a, v3 normal){
    f32 dot = dot_product_v3(a, normal);
    return(a - ((2.0f * dot) * normal));
}

// UNTSTED
static void
swap_v2(v2* a, v2* b){
    v2 t = *a;
    *a = *b;
    *b = t;
}

#endif
