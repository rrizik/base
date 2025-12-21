#if !defined(BASE_LINKEDLIST_H)
#define BASE_LINKEDLIST_H

#include "base_types.h"
#include "base_memory.h"

///////////////////////////////
// NOTE: Linked List & Doubly Linked List (Not circular)
//
// - This is a Linked List with a Sentinel implementation. IMPORTANT: Sentinel is being removed
//   It is expected that you dll_init() or dll_clear() before you can use the sentinel.
// - DLL here can be treated as DLL, SLL, Queue, Stack out of convenience, although not optimal.
// - SLL stack should have only 1 next pointer.
///////////////////////////////

///////////////////////////////
// NOTE: Linked List
///////////////////////////////

#define sll_push_front(head, node) do { \
    (node)->next = (head);              \
    (head) = (node);                    \
} while(0)

#define sll_pop_front(head) do { \
    (head) = (head)->next;       \
} while(0)

#define sll_clear(head) do { \
    (head) = 0;              \
} while(0)

///////////////////////////////
// NOTE: Doubly Linked List
///////////////////////////////

#define dll_push_front(list, node) do {         \
    (node)->prev = 0;                           \
    (node)->next = (list)->first;               \
                                                \
    if((list)->first)                           \
        (list)->first->prev = (node);           \
    else                                        \
        (list)->last = (node);                  \
                                                \
    (list)->first = (node);                     \
    (list)->node_count += 1;                    \
    (list)->total_size += (node)->string.count; \
} while (0)

#define dll_push_back(list, node) do {          \
    (node)->next = 0;                           \
    (node)->prev = (list)->last;                \
                                                \
    if((list)->last)                            \
        (list)->last->next = (node);            \
    else                                        \
        (list)->first = (node);                 \
                                                \
    (list)->last = (node);                      \
    (list)->node_count += 1;                    \
    (list)->total_size += (node)->string.count; \
} while (0)

#define dll_pop_front(list) do {               \
    String8Node *n = (list)->first;            \
    if(n) {                                    \
        (list)->first = n->next;               \
        if((list)->first)                      \
            (list)->first->prev = 0;           \
        else                                   \
            (list)->last = 0;                  \
                                               \
        n->next = n->prev = 0;                 \
        (list)->node_count -= 1;               \
        (list)->total_size -= n->string.count; \
    }                                          \
} while (0)

#define dll_pop_back(list) do {                \
    String8Node *n = (list)->last;             \
    if(n) {                                    \
        (list)->last = n->prev;                \
        if((list)->last)                       \
            (list)->last->next = 0;            \
        else                                   \
            (list)->first = 0;                 \
                                               \
        n->next = n->prev = 0;                 \
        (list)->node_count -= 1;               \
        (list)->total_size -= n->string.count; \
    }                                          \
} while (0)

#define dll_remove(list, node) do {             \
    if((node)->prev)                            \
        (node)->prev->next = (node)->next;      \
    else                                        \
        (list)->first = (node)->next;           \
                                                \
    if((node)->next)                            \
        (node)->next->prev = (node)->prev;      \
    else                                        \
        (list)->last = (node)->prev;            \
                                                \
    (node)->prev = 0;                           \
    (node)->next = 0;                           \
    (list)->node_count -= 1;                    \
    (list)->total_size -= (node)->string.count; \
} while (0)

///////////// OLD STUFF //////////////

#define dll_push_front_old(list, node) ((((node)->prev)=(list)),\
                                    (((node)->next)=((list)->next)),\
                                    (((node)->prev->next)=(node)),\
                                    (((node)->next->prev)=(node)))

#define dll_push_back_old(list, node) ((((node)->prev)=((list)->prev)),\
                                   (((node)->next)=(list)),\
                                   (((node)->prev->next)=(node)),\
                                   (((node)->next->prev)=(node)))

#define dll_pop_front_old(list) ((((list)->next->next->prev)=(list)),\
                             (((list)->next)=((list)->next->next)))

#define dll_pop_back_old(list) ((((list)->prev->prev->next)=(list)),\
                            (((list)->prev)=((list)->prev->prev)))

#define dll_remove_old(node) ((((node)->prev->next)=((node)->next)),\
                          (((node)->next->prev)=((node)->prev)),\
                          (((node)->next)=(node)),\
                          (((node)->prev)=(node)))

#define dll_init(list) dll_clear(list)
#define dll_clear(list) ((((list)->next)=(list)),\
                         (((list)->prev)=(list)))

#define dll_swap(n1, n2, Type) (n1)->prev->next = (n2); \
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


#endif
