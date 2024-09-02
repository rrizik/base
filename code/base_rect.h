#ifndef RECT_H
#define RECT_H

// NOTE: Rect by default is in screenspace
typedef union Rect {
    struct {
        f32 x0, y0;
        f32 x1, y1;
    };
    struct {
        v2 min;
        v2 max;
    };
} Rect;

static Rect
make_rect(f32 x0, f32 y0, f32 x1, f32 y1){
    Rect result;
    result.x0 = x0;
    result.y0 = y0;
    result.x1 = x1;
    result.y1 = y1;
    return(result);
}

static Rect
make_rect(v2 min, v2 max){
    Rect result;
    result.min = min;
    result.max = max;
    return(result);
}

static Rect
make_rect_size(v2 min, v2 size){
    Rect result;
    result.min = min;
    result.max = make_v2(min.x + size.x, min.y + size.y);
    return(result);
}

static bool
rect_collides_rect(Rect rect1, Rect rect2) {
    if (rect1.x1 < rect2.x0 || rect1.x0 > rect2.x1 || rect1.y1 < rect2.y0 || rect1.y0 > rect2.y1) {
        return false;
    }
    return true;
}

static bool
rect_contains_point(Rect r1, v2 p){
    if((p.x > r1.x0) &&
       (p.x < r1.x1) &&
       (p.y > r1.y0) &&
       (p.y < r1.y1)){
        return true;
    }
    return false;
}

static bool
rect_contains_point(Rect r1, v2s32 p){
    if(((f32)p.x > r1.x0) &&
       ((f32)p.x < r1.x1) &&
       ((f32)p.y > r1.y0) &&
       ((f32)p.y < r1.y1)){
        return true;
    }
    return false;
}

static bool
rect_contains_rect(Rect r1, Rect r2){
    if((r2.x0 > r1.x0) &&
       (r2.x0 + r2.x1 < r1.x0 + r1.x1) &&
       (r2.y0 > r1.y0) &&
       (r2.y0 + r2.y1 < r1.y0 + r1.y1)){
        return true;
    }
    return false;
}

static v2
rect_width_height(Rect rect){

    v2 result = rect.max - rect.min;
    return(result);
}

static Rect
rect_screen_from_clip(Rect r, v2s32 res){
    Rect result = {
        ((r.x0  + 1.0f) / 2.0f) * (f32)res.w,
        ((r.y0  + 1.0f) / 2.0f) * (f32)res.h,
        ((r.x1  + 1.0f) / 2.0f) * (f32)res.w,
        ((r.y1  + 1.0f) / 2.0f) * (f32)res.h,
    };
    return(result);
}

static Rect
rect_clip_from_screen(Rect r, v2s32 res){
    Rect result = {
        ((r.x0 / (f32)res.w) * 2.0f) - 1.0f,
        ((r.y0 / (f32)res.h) * 2.0f) - 1.0f,
        ((r.x1 / (f32)res.w) * 2.0f) - 1.0f,
        ((r.y1 / (f32)res.h) * 2.0f) - 1.0f,
    };
    return(result);
}

static Rect
rect_clip_from_screen_inverted(Rect r, v2s32 res){
    Rect result = {
       (((r.x0 / (f32)res.w) * 2.0f) - 1.0f),
      -(((r.y0 / (f32)res.h) * 2.0f) - 1.0f),
       (((r.x1 / (f32)res.w) * 2.0f) - 1.0f),
      -(((r.y1 / (f32)res.h) * 2.0f) - 1.0f)
    };
    return(result);
}

#endif
