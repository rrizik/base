
typedef struct Window{
    union{
        struct{
            f32 width;
            f32 height;
        };
        v2 dim;
    };

    HWND handle;
} Window;

static v2s32
os_mouse_pos(Window* window){
    v2s32 result = {s32_min, s32_min};
    if(!window->handle){
        return(result);
    }

    POINT point;
    if(!GetCursorPos(&point)){
        return(result);
    }

    if(!ScreenToClient(window->handle, &point)){
        return(result);
    }

    result = make_v2s32(point.x, point.y);
    return(result);
}
