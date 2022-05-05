

typedef struct PermanentState{
    Arena arena;
} PermanentState, State;

typedef struct TransientState{
    Arena arena;
} TransientState;

static void render_something(RenderBuffer* render_buffer){
    u8* row = (u8*)render_buffer->base;
    for(i32 y=0; y < render_buffer->height; ++y){
        u32* pixel = (u32*)row;
        for(i32 x=0; x < render_buffer->width; ++x){
            *pixel++ = (100 << 16|100);
        }
        row += render_buffer->stride;
    }
}

static void update_game(Memory* memory, RenderBuffer* render_buffer, Controller* controller, Clock* clock){

    Assert(sizeof(PermanentState) < memory->permanent_size);
    Assert(sizeof(TransientState) < memory->permanent_size);
    PermanentState* permanent_state = (PermanentState*)memory->permanent_base;
    TransientState* transient_state = (TransientState*)memory->transient_base;

    if(memory->initialized){
        memory->initialized = true;
        arena_init(&permanent_state->arena, (u8*)memory->permanent_base + sizeof(PermanentState), memory->permanent_size - sizeof(PermanentState));
        arena_init(&transient_state->arena, (u8*)memory->transient_base + sizeof(TransientState), memory->transient_size - sizeof(TransientState));
    }

    render_something(render_buffer);
}
