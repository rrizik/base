#ifndef BASE_MATH_H
#define BASE_MATH_H

#include <math.h>
#include "base_types.h"
#include "base_vector.h"
#include "base_quad.h"
#include "base_rect.h"
// UNTESTED: re test all the rounding/trunct/floor/ceil

///////////////////////////////
// NOTE: Math Functions
///////////////////////////////

// TODO: replace these with functions for less bugs and better debuggability
#define MIN(a,b) (((a)<=(b))?(a):(b))
#define MAX(a,b) (((a)>=(b))?(a):(b))
#define CLAMP(a,x,b) (((x)<(a))?(a):((x)>(b))?(b):(x))
#define AlignUpPow2(x,p) (((x) + (p) - 1)&~((p) - 1))
#define AlignDownPow2(x,p) ((x)&~((p) - 1))

static f32
square_f32(f32 value){
    f32 result = value * value;
    return(result);
}

static f64
square_f64(f64 value){
    f64 result = value * value;
    return(result);
}

static f32
sqrt_f32(f32 x){
    return(sqrtf(x));
}

static f32
sin_f32(f32 theta){
    f32 result = sinf(theta);
    return(result);
}

static f32
cos_f32(f32 theta){
    f32 result = cosf(theta);
    return(result);
}

static f32
tan_f32(f32 theta){
    //f32 result = tanf(theta);
    //f32 r = sin_f32(theta)/cos_f32(theta);
    return(sin_f32(theta)/cos_f32(theta));
}
static f32 cot_f32(f32 theta){ return(cos_f32(theta)/sin_f32(theta)); }
static f32 atan_f32(f32 x, f32 y){
    f32 result = atan2f(x, y);
    return(result);
}

static f64 sqrt_f64(f64 x){ return(sqrt(x)); }
static f64 sin_f64(f64 x){ return(sin(x)); }
static f64 cos_f64(f64 x){ return(cos(x)); }
static f64 tan_f64(f64 x){ return(sin_f64(x)/cos_f64(x)); }
static f64 cot_f64(f64 x){ return(cos_f64(x)/sin_f64(x)); }
static f64 atan_f64(f64 x, f64 y){ return(atan2(x, y)); }

#define PI_f32 3.14159265359f
#define PI_f64 3.14159265359
#define RAD_f32 0.0174533f
#define RAD_f64 0.0174533

static f32
rad_from_deg(f32 deg){
    f32 result = ((PI_f32/180.0f) * deg);
    return(result);
}

static f64
rad_from_deg_f64(f64 deg){
    f64 result = ((PI_f32/180.0f) * deg);
    return(result);
}

static f32
deg_from_rad(f32 rad){
    f32 result = ((180.0f/PI_f32) * rad);
    return(result);
}

static v2
dir_from_rad(f32 rad){
    v2 result = {cos_f32(rad), sin_f32(rad)};
    return(result);
}

static f32
rad_from_dir(v2 dir){
    f32 result = atan_f32(dir.y, dir.x);
    return(result);
}

static v2
dir_from_deg(f32 deg){
    f32 rad = rad_from_deg(deg);
    v2 result = {cos_f32(rad), sin_f32(rad)};
    return(result);
}

static f32
deg_from_dir(v2 dir){
    f32 rad = atan_f32(dir.y, dir.x);
    f32 result = deg_from_rad(rad);
    return(result);
}

static f32
abs_f32(f32 x){
    union{ f32 f; u32 u; } r;
    r.f = x;
    r.u &= 0x7fffffff;
    return(r.f);
}

static f64
abs_f64(f64 x){
    union{ f64 f; u64 u; } r;
    r.f = x;
    r.u &= 0x7fffffffffffffff;
    return(r.f);
}

static s32
abs_s32(s32 x){
    s32 result = ((x<0)?(-x):(x));
    return(result);
}

static s64
abs_s64(s64 x){
    s64 result = ((x<0)?(-x):(x));
    return(result);
}

static f32
round_f32(f32 value){
    f32 result = (f32)((s32)(value + 0.5f));
    return(result);
}

// todo: this seems like something that I don't want
static v2
round_f32(v2 value){
    v2 result = {
        round_f32(value.x),
        round_f32(value.y),
    };
    return(result);
}

static f64
round_f64(f64 value){
    f64 result = (f64)((s64)(value + 0.5));
    return(result);
}

static s32
round_f32_s32(f32 value){
    s32 result = (s32)roundf(value);
    return(result);
}

static s64
round_f64_s64(f64 value){
    s64 result = (s64)roundl(value);
    return(result);
}

// TODO: Does this even make sense?
static u32
round_f32_u32(f32 value){
    u32 result = (u32)roundf(value);
    return(result);
}

static f32
truncate_f32(f32 value){
    f32 result = (f32)(s32)value;
    return(result);
}

static s32
truncate_f32_s32(f32 value){
    s32 result = (s32)value;
    return(result);
}

static f32
floor_f32(f32 value){
    f32 result = floorf(value);
    return(result);
}

static s32
floor_f32_s32(f32 value){
    s32 result = (s32)floorf(value);
    return(result);
}

static f32
ceil_f32(f32 value){
    f32 result = ceilf(value);
    return(result);
}

static s32
ceil_f32_s32(f32 value){
    s32 result = (s32)floorf(value);
    return(result);
}

static void
clamp_f32(f32 left, f32 right, f32* value){
    if(*value < left) { *value = left; }
    if(*value > right) { *value = right; }
}

static void
clamp_u32(u32 left, u32 right, u32* value){
    if(*value < left) { *value = left; }
    if(*value > right) { *value = right; }
}

static void
clamp_s32(s32 left, s32 right, s32* value){
    if(*value < left) { *value = left; }
    if(*value > right) { *value = right; }
}

// UNTESTED:
static void
clamp_f32_s32(f32 left, f32 right, f32* value){
    if(*value < left) { *value = left; }
    if(*value > right) { *value = right; }
}

///////////////////////////////
// NOTE: Lerp & Easing
///////////////////////////////

static f32
lerp(f32 a, f32 b, f32 t){
    f32 result = ((1.0f - t) * a) + (t * b);
    return(result);
}

static RGBA
lerp(RGBA a, RGBA b, f32 t){
    RGBA result = ((1.0f - t) * a) + (t * b);
    return(result);
}

static f32
unlerp(f32 a, f32 b, f32 at){
    f32 t = 0.0f;
    if(a != b){
        t = (at - a) / (b - a);
    }
    return(t);
}

// UNTESTED:
// NOTE: Spherical linear interpolation
static f32
slerp_f32(f32 a, f32 b, f32 t) {
    f32 difference = fmodf(b - a, 2 * PI_f32);
    f32 distance = fmodf(2.0f * difference, 2 * PI_f32) - difference;
    return a + distance * t;
}

// UNTESTED:
static v2
slerp_v2(v2 a, f32 t, v2 b) {
    a = normalize_v2(a);
    b = normalize_v2(b);

    f32 dot = dot_product_v2(a, b);
    clamp_f32(-1.0, 1.0, &dot);

    f32 theta = acosf(dot) * t;
    v2 relative_vector = normalize_v2(b - a * dot);
    v2 result = (a * cos_f32(theta)) + (relative_vector * sin_f32(theta));
    return(result);
}

static f32
smoothstep(f32 t){
    f32 value1 = t * t;
    f32 value2 = 1.0f - (1.0f - t) * (1.0f - t);
    return(lerp(value1, value2, t));
}

static f32
ease_out(f32 t){
    return(sqrtf(1 - powf(t - 1, 4)));
}

///////////////////////////////
// NOTE: Other
///////////////////////////////

static v2
rotate_point_rad(v2 p, f32 rad, v2 origin){
    v2 result = {0};

    result.x = (p.x - origin.x) * cos_f32(rad) - (p.y - origin.y) * sin_f32(rad) + origin.x;
    result.y = (p.x - origin.x) * sin_f32(rad) + (p.y - origin.y) * cos_f32(rad) + origin.y;

    return(result);
}

static v2
rotate_point(v2 p, f32 deg, v2 origin){
    v2 result = {0};

    f32 rad = rad_from_deg(deg);
    result.x = (p.x - origin.x) * cos_f32(rad) - (p.y - origin.y) * sin_f32(rad) + origin.x;
    result.y = (p.x - origin.x) * sin_f32(rad) + (p.y - origin.y) * cos_f32(rad) + origin.y;

    return(result);
}

static Quad
rotate_quad(Quad quad, f32 deg, v2 origin){
    Quad result = {0};

    f32 rad = rad_from_deg(-deg);
    result.p0 = rotate_point(quad.p0, deg, origin);
    result.p1 = rotate_point(quad.p1, deg, origin);
    result.p2 = rotate_point(quad.p2, deg, origin);
    result.p3 = rotate_point(quad.p3, deg, origin);

    return(result);
}

static f32
wrap_degrees(f32 deg){
    while (deg <= -360) deg += 360;
    while (deg >= 360) deg -= 360;
    return(deg);
}

static void
wrap_degrees(f32* deg){
    while (*deg <= 0) *deg += 360;
    while (*deg >= 360) *deg -= 360;
}

// todo: it doesn't make sense to rotate a rect
static Rect
rotate_rect(Rect rect, f32 deg, v2 origin){
    Rect result = {0};

    f32 rad = rad_from_deg(-deg);
    result.min = rotate_point(rect.min, deg, origin);
    result.max = rotate_point(rect.max, deg, origin);

    return(result);
}

// todo: this doesn't make sense. quad center but takes in min, max.
static v2
quad_center(v2 min, v2 max){
    v2 result;
    result.x = (min.x + max.x) * 0.5f;
    result.y = (min.y + max.y) * 0.5f;
    return(result);
}


#endif
