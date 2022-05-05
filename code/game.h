

typedef struct PermanentMemory{
    Arena arena;
} PermanentMemory;

typedef struct TransientMemory{
    Arena arena;
    DLL stuff;
} TransientMemory;

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

global PermanentMemory* permanent_memory;
global TransientMemory* transient_memory;

static void update_game(Memory* memory, RenderBuffer* render_buffer, Controller* controller, Clock* clock){

    Assert(sizeof(PermanentMemory) < memory->permanent_size);
    Assert(sizeof(TransientMemory) < memory->transient_size);
    permanent_memory = (PermanentMemory*)memory->permanent_base;
    transient_memory = (TransientMemory*)memory->transient_base;

    if(memory->initialized){
        memory->initialized = true;
        arena_init(&permanent_memory->arena, (u8*)memory->permanent_base + sizeof(PermanentMemory), memory->permanent_size - sizeof(PermanentMemory));
        arena_init(&transient_memory->arena, (u8*)memory->transient_base + sizeof(TransientMemory), memory->transient_size - sizeof(TransientMemory));
    }

    render_something(render_buffer);
}
