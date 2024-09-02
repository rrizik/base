#if !defined(BASE_LINKEDLIST_H)
#define BASE_LINKEDLIST_H

#include "base_types.h"
#include "base_memory.h"

///////////////////////////////
// NOTE: Doubly Linked List
//
// - This is a Linked List with a Sentinel implementation.
//   It is expected that you dll_init() or dll_clear() before you can use the sentinel.
// - DLL here can be treated as DLL, SLL, Queue, Stack out of convenience, although not optimal.
// - SLL stack should have only 1 next pointer.
///////////////////////////////


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
