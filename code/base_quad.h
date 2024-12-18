#ifndef QAUD_H
#define QAUD_H

typedef union Quad{
    struct{
        v2 p0, p1, p2, p3;
    };
    struct{
        v2 top_left, top_right, bottom_right, bottom_left;
    };
    struct{
        v2 p00, p10, p11, p01;
    };
} Quad;

static Quad
make_quad(v2 p0, v2 p1, v2 p2, v2 p3){
    Quad result;
    result.p0 = p0;
    result.p1 = p1;
    result.p2 = p2;
    result.p3 = p3;
    return(result);
}

static Quad
make_quad(v2 pos, v2 dim){
    Quad result;
    result.p0 = make_v2(pos.x - dim.w/2, pos.y - dim.h/2);
    result.p1 = make_v2(pos.x + dim.w/2, pos.y - dim.h/2);
    result.p2 = make_v2(pos.x + dim.w/2, pos.y + dim.h/2);
    result.p3 = make_v2(pos.x - dim.w/2, pos.y + dim.h/2);
    return(result);
}

static Quad
quad_clip_from_screen(Quad q, v2s32 res){
    v2 p0 = {((q.p0.x / (f32)res.w) * 2.0f) - 1.0f, ((q.p0.y / (f32)res.h) * 2.0f) - 1.0f};
    v2 p1 = {((q.p1.x / (f32)res.w) * 2.0f) - 1.0f, ((q.p1.y / (f32)res.h) * 2.0f) - 1.0f};
    v2 p2 = {((q.p2.x / (f32)res.w) * 2.0f) - 1.0f, ((q.p2.y / (f32)res.h) * 2.0f) - 1.0f};
    v2 p3 = {((q.p3.x / (f32)res.w) * 2.0f) - 1.0f, ((q.p3.y / (f32)res.h) * 2.0f) - 1.0f};

    Quad result = make_quad(p0, p1, p2, p3);
    return(result);
}


static Quad quad_from_pos_center(v2 pos, v2 dim){
    Quad result = {0};
    result.p0 = make_v2(pos.x - dim.w/2, pos.y - dim.h/2);
    result.p1 = make_v2(pos.x + dim.w/2, pos.y - dim.h/2);
    result.p2 = make_v2(pos.x + dim.w/2, pos.y + dim.h/2);
    result.p3 = make_v2(pos.x - dim.w/2, pos.y + dim.h/2);

    return(result);
}

static Quad quad_from_pos(v2 pos, v2 dim){
    Quad result = {0};
    result.p0 = make_v2(pos.x, pos.y);
    result.p1 = make_v2(pos.x + dim.w, pos.y);
    result.p2 = make_v2(pos.x + dim.w, pos.y + dim.h);
    result.p3 = make_v2(pos.x, pos.y + dim.h);

    return(result);
}

#endif

