#if !defined(BASE_LINKEDLIST_H)
#define BASE_LINKEDLIST_H

#include "base_types.h"
#include "base_memory.h"

///////////////////////////////
// NOTE: Doubly Linked List
///////////////////////////////
// NOTE: DLL here can be treated as DLL, SLL, Queue, Stack
// out of convenience, although not optimal. SLL stack should
// have only 1 next pointer

// TODO: Make it so that you don't have to initialize next prev as part of the macros
#define sll_push_front(list, node) (((node)->next)=(list->next),\
                                    ((list)->next)=(node))

#define sll_pop_front(list) ((((list)->next)=((list)->next->next)))

#define dll_push_front(list, node) ((((node)->prev)=(list)),\
                                    (((node)->next)=((list)->next)),\
                                    (((node)->prev->next)=(node)),\
                                    (((node)->next->prev)=(node)))

#define dll_push_back(list, node) ((((node)->prev)=((list)->prev)),\
                                   (((node)->next)=(list)),\
                                   (((node)->prev->next)=(node)),\
                                   (((node)->next->prev)=(node)))

#define dll_pop_front(list) ((((list)->next->next->prev)=(list)),\
                             (((list)->next)=((list)->next->next)))

#define dll_pop_back(list) ((((list)->prev->prev->next)=(list)),\
                            (((list)->prev)=((list)->prev->prev)))

#define dll_remove(node) ((((node)->prev->next)=((node)->next)),\
                          (((node)->next->prev)=((node)->prev)),\
                          (((node)->next)=(node)),\
                          (((node)->prev)=(node)))

#define dll_clear(list) ((((list)->next)=(list)),\
                         (((list)->prev)=(list)))

// todo: dll_insert(list, node, prev_node)

//#define dll_node_from_idx(list, idx)\
//    CCategory* c = pm->categories; \
//    for(s32 i=0; i <= from_idx; ++i){ \
//        c = c->next; \
//    } \
//    return(c);


#define dll_swap(n1, n2, Type) \
    (n1)->prev->next = (n2); \
    (n2)->prev->next = (n1); \
                             \
    Type* prev_node = (n1)->prev; \
    (n1)->prev = (n2)->prev; \
    (n2)->prev = prev_node; \
                            \
    Type* next_node = (n1)->next; \
    (n1)->next = (n2)->next; \
    (n2)->next = next_node; \
                            \
    (n1)->next->prev = (n1); \
    (n2)->next->prev = (n2);


//typedef struct Node{
//    struct Node* next;
//    struct Node* prev;
//    void* data;
//    u32 count;
//} LLNode, DLL, SLL, Queue, Stack, String8Node;
//
//
//static void dll_push_front(Node* sentinel, Node* node){
//    node->prev = sentinel;
//    node->next = sentinel->next;
//
//    node->prev->next = node;
//    node->next->prev = node;
//    sentinel->count++;
//}
//
//static void dll_push_back(Node* sentinel, Node* node){
//    node->prev = sentinel->prev;
//    node->next = sentinel;
//
//    node->prev->next = node;
//    node->next->prev = node;
//    sentinel->count++;
//}
//
//static Node* dll_pop_front(Node* sentinel){
//    Node* node = sentinel->next;
//    node->prev->next = node->next;
//    node->next->prev = node->prev;
//    node->next = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
//    node->prev = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
//    sentinel->count--;
//    return(node);
//}
//
//static Node* dll_pop_back(Node* sentinel){
//    Node* node = sentinel->prev;
//    node->prev->next = node->next;
//    node->next->prev = node->prev;
//    node->next = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
//    node->prev = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
//    sentinel->count--;
//    return(node);
//}
//
//static void dll_remove(Node* sentinel, Node* node){
//    node->prev->next = node->next;
//    node->next->prev = node->prev;
//    node->next = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
//    node->prev = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
//    sentinel->count--;
//}
//
//static void dll_clear(DLL *sentinel){
//    sentinel->count = 0;
//    sentinel->next = sentinel;
//    sentinel->prev = sentinel;
//    sentinel->data = NULL;
//}

#endif
