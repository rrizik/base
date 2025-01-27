#include "base_inc.h"
#include "win32_base_inc.h"


Arena* arena = make_arena(KB(1));

s32 main(s32 argc, char** argv){
    ScratchArena scratch = begin_scratch();

    String8 value = str8_literal("d");
    String8 path = str8_literal("a/b/c");
    String8Node* parts = str8_split(scratch.arena, path, '/');
    str8_list_push_back(scratch.arena, parts, value);

    //String8Node* parts_view = &parts;
    //print("Sentinel: %p\n", parts_view);
    print("Sentinel: %p\n", parts);
    //print("Sentinel: %p\n", &parts);
    for(String8Node* part = parts->prev; part != parts; part = part->prev){
        print("Data: %s, Node: %p, Next: %p, Prev: %p\n", part->str.data, part, part->next, part->prev);
    }
    print("-----------------------\n");
    for(String8Node* part = parts->next; part != parts; part = part->next){
        print("Data: %s, Node: %p, Next: %p, Prev: %p\n", part->str.data, part, part->next, part->prev);
    }

    String8Join join = {0};
    join.mid = str8_literal("/");
    String8 result = str8_join(arena, parts, join);

    end_scratch(scratch);

    //String8 a = str8_literal("a/b/c");
    //String8 full_a = str8_path_append(arena, a, str8_literal("d"));

    //String8Node parts = str8_split(arena, a, '/');

    ////String8Node* parts_view = &parts;
    ////print("Sentinel: %p\n", parts_view);
    ////for(String8Node* part = parts_view->next; part != parts_view; part = part->next){
    ////    print("Data: %s, Node: %p, Next: %p, Prev: %p\n", part->str.data, part, part->next, part->prev);
    ////}

    //print("Sentinel: %p\n", &parts);
    //for(String8Node* part = parts.next; part != &parts; part = part->next){
    //    print("Data: %s, Node: %p, Next: %p, Prev: %p\n", part->str.data, part, part->next, part->prev);
    //}

    //String8Join join = {0};
    //join.mid = str8_literal("/");
    //String8 result = str8_join(arena, &parts, join);
    //print("%s\n", result.data);

    return(0);
}
