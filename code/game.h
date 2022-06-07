

typedef struct PermanentMemory{
    Arena arena;
    String8 cwd; // CONSIDER: this might be something we want to be set on the platform side
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


static void update_game(Memory* memory, RenderBuffer* render_buffer, Controller* controller, Clock* clock){

    Assert(sizeof(PermanentMemory) < memory->permanent_size);
    Assert(sizeof(TransientMemory) < memory->transient_size);
    PermanentMemory* PM = (PermanentMemory*)memory->permanent_base;
    TransientMemory* TM = (TransientMemory*)memory->transient_base;

    if(!memory->initialized){
        v2 a = {1, 1};
        v2 b = {1, 1};
        v2 c = a + b;
        memory->initialized = true;
        arena_init(&PM->arena, (u8*)memory->permanent_base + sizeof(PermanentMemory), memory->permanent_size - sizeof(PermanentMemory));
        arena_init(&TM->arena, (u8*)memory->transient_base + sizeof(TransientMemory), memory->transient_size - sizeof(TransientMemory));
        PM->cwd = os_get_cwd(&TM->arena);
    }

    render_something(render_buffer);
}
