#include "base_inc.h"
#include "win32_base_inc.h"

typedef struct Test{
    Test* next;
    Test* prev;
    u32 a;
    u32 b;
} Test;

s32 main(s32 argc, char** argv){
    PoolArena* pool = make_pool(sizeof(Test), 3);
    pool_free_all(pool);

    Test* a = (Test*)pool_next(pool);
    a->a = 1;
    a->b = 2;
    Test* b = (Test*)pool_next(pool);
    b->a = 3;
    b->b = 4;
    Test* c = (Test*)pool_next(pool);
    c->a = 5;
    c->b = 6;
    pool_free(pool, a);
    Test* d = (Test*)pool_next(pool);
    d->a = 7;
    d->b = 8;
    pool_free(pool, a);
    Test* e = (Test*)pool_next(pool);
    e->a = 9;
    e->b = 10;
    pool_free(pool, b);
    Test* f = (Test*)pool_next(pool);
    f->a = 11;
    f->b = 12;

    pool_free_all(pool);



    Test one = {0};
    one.a = 100;
    one.b = 200;
    dll_clear(&one);
    Test two = {0};
    two.a = 300;
    two.b = 400;
    dll_push_back(&one, &two);
    dll_remove(&two);
    dll_remove(&two);

    return(0);
}
