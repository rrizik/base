

static v2s32
os_mouse_pos(HWND handle){
    v2s32 result = {s32_min, s32_min};
    if(!handle){
        return(result);
    }

    POINT point;
    if(!GetCursorPos(&point)){
        return(result);
    }

    if(!ScreenToClient(handle, &point)){
        return(result);
    }

    result = make_v2s32(point.x, point.y);
    return(result);
}
