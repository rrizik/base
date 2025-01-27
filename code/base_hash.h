#ifndef BASE_HASH_H
#define BASE_HASH_H

// note: important: This generic implementation is super restrictive and doesn't solve anything. I will likely need many different kinds of hash tables soon and this immediately will be invalid and unusable. Its likely best to write the specific hash table for the specific problem at hand, rather than writing a generic one like this.

// note: should only allocate 1 type per-table
typedef struct HashNode{
    HashNode* next;
    u64 hash;
    String8 key;
    u8 value[];
} HashNode;

typedef struct HashTable{
    Arena* arena;
    u64 count;
    HashNode** slots;
} HashTable;

// note: good enough to get started
static u64
hash_from_string(String8 string){
    u64 result = 5381;

    for(u64 i=0; i < string.count; ++i){
        result = ((result << 5) + result) + string.data[i];
    }

    return(result);
}

#define TABLE_DEFAULT_COUNT 1024
static void
table_init(Arena* arena, HashTable* table){
    table->arena = arena;
    table->count = TABLE_DEFAULT_COUNT;
    table->slots = push_array_zero(table->arena, HashNode*, table->count);
}

static String8
string_from_hash(HashTable* table, u64 hash){
    u64 index = hash % table->count;
    HashNode* node = table->slots[index];
    if(node){
        return(node->key);
    }
    return(str8_literal(""));
}

#define table_insert(table, key, value) table_insert_((table), key, &(value), sizeof(value))
static void
table_insert_(HashTable* table, String8 key, void* value, u64 size){
    // if new table, allocate default amount
    if(table->count == 0){
        table->count = TABLE_DEFAULT_COUNT;
        table->slots = push_array_zero(table->arena, HashNode*, table->count);
    }

    u64 hash = hash_from_string(key);
    u64 slot_idx = hash % table->count;
    HashNode* found_node = 0;

    HashNode* n = table->slots[slot_idx];
    while(n != 0){
        if(n->hash == hash && str8_compare(n->key, key)){
            found_node = n;
            break;
        }
        n = n->next;
    }

    // overwrite found node.
    if(found_node != 0){
        memcpy(found_node->value, value, size);
    }

    // node not found. create new one.
    if(found_node == 0){
        HashNode* node = (HashNode*)push_size_aligned(table->arena, (u32)(sizeof(HashNode) + size), (u32)alignof(HashNode));
        memcpy(node->value, value, size);

        node->hash = hash;
        node->key = key;
        node->next = table->slots[slot_idx];
        table->slots[slot_idx] = node;
    }
}

#define table_lookup(type, table, key) (type*)table_lookup_(table, key)
static void*
table_lookup_(HashTable* table, String8 key){
    u64 hash = hash_from_string(key);
    u64 slot_idx = hash % table->count;
    HashNode* found_node = 0;

    HashNode* n = table->slots[slot_idx];
    while(n != 0){
        if(n->hash == hash && str8_compare(n->key, key)){
            found_node = n;
            break;
        }
        n = n->next;
    }

    void* result = {0};
    if(found_node != 0){
        result = found_node->value;
    }
    return(result);
}


#endif
