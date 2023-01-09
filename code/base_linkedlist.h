#if !defined(BASE_LINKEDLIST_H)
#define BASE_LINKEDLIST_H

#include "base_types.h"
#include "base_memory.h"

///////////////////////////////
// NOTE: Doubly Linked List
///////////////////////////////
// NOTE: DLL here can be treated as DLL, SLL, Queue, Stack
// out of convenience, although not optimal. Each should
// have its own implementation

typedef struct Node{
    struct Node* next;
    struct Node* prev;
    void* data;
    u32 count;
} LLNode, DLL, SLL, Queue, Stack;

static Node* push_node(Arena* arena){
    Node* result = push_struct(arena, Node);
    result->next = result;
    result->prev = result;
    result->count = 0;
    return(result);
}

static void dll_push_front(Node* sentinel, Node* node){
    node->prev = sentinel;
    node->next = sentinel->next;

    node->prev->next = node;
    node->next->prev = node;
    sentinel->count++;
}

static void dll_push_back(Node* sentinel, Node* node){
    node->prev = sentinel->prev;
    node->next = sentinel;

    node->prev->next = node;
    node->next->prev = node;
    sentinel->count++;
}

static Node* dll_pop_front(Node* sentinel){
    Node* node = sentinel->next;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
    node->prev = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
    sentinel->count--;
    return(node);
}

static Node* dll_pop_back(Node* sentinel){
    Node* node = sentinel->prev;
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
    node->prev = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
    sentinel->count--;
    return(node);
}

static void dll_remove(Node* sentinel, Node* node){
    node->prev->next = node->next;
    node->next->prev = node->prev;
    node->next = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
    node->prev = node; // QUESTION: gaurd against node referncing nodes in linked list? is this even necessay?
    sentinel->count--;
}

static void reset_sentinel(DLL *sentinel){
    sentinel->count = 0;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;
    sentinel->data = NULL;
}

#endif
