#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashes.h"


typedef struct HAPHashTableNode HAPHashTableNode;
typedef struct HAPHashTable HAPHashTable;


struct HAPHashTableNode {
    void *key;
    void *value;
    HAPHashTableNode *next;
};


HAPHashTableNode * hap_hash_table_node_create(void * const key, void * const value) {
    HAPHashTableNode *node = calloc(1, sizeof(HAPHashTableNode*));
    if (node == NULL) return NULL;

    (*node).key = key;
    (*node).value = value;

    return node;
}


void hap_hash_table_node_destroy(HAPHashTableNode *node) {
    if (node == NULL) return;
    free(node);
}


struct HAPHashTable {
    HAPHashTableNode *(*partitions)[];

    bool fixed;
    unsigned long size;
};


void* hap_hash_table_create(unsigned long size, bool fixed) {
    HAPHashTable *table = calloc(1, sizeof(HAPHashTable*));
    if (table == NULL) return NULL;

    (*table).partitions = calloc(size, sizeof(HAPHashTableNode*));

    if ((*table).partitions == NULL) {
        free(table);
        return NULL;
    }

    (void)fixed;  // TODO: Add support for dynamically resizing buckets when fixed is false
    (*table).fixed = true;

    return table;
}


void hap_hash_table_destroy(void *table) {
    unsigned long index;

    HAPHashTable *subject = (HAPHashTable*) table;

    // Failed to cast. We were given something silly.
    if (subject == NULL) return;

    for (index = 0; index < (*subject).size; ++index)
        hap_hash_table_node_destroy((*(*subject).partitions)[index]);

    free(table);
}


unsigned long get_partition_index(HAPHashTable *table, void *key) {
    // TODO: Account for modulo bias... Maybe?
    return fmod(hash_ptr(key), (*table).size);
}


HAPHashTableNode* hap_hash_table_node_find(void *table, void *key, bool getPreceding) {
    HAPHashTableNode *current = NULL;
    HAPHashTable *subject = (HAPHashTable*) table;

    unsigned long index = get_partition_index(table, key);

    // Failed to cast. We were given something silly.
    if (subject == NULL) return NULL;

    current = (*(*subject).partitions)[index];

    while (current) {
        if (getPreceding && (*current).next && (*(*current).next).key == key) return current;
        if ((*current).key == key) return current;
        current = (*current).next;
    }

    return NULL;
}


void * hap_hash_table_find(void *table, void *key) {
    HAPHashTableNode* node = hap_hash_table_node_find(table, key, false);
    if (node == NULL) return NULL;
    return (*node).value;
}


void* hap_hash_table_remove(void *table, void *key) {
    void *result;
    unsigned long index;

    HAPHashTable *subject = (HAPHashTable*) table;
    HAPHashTableNode *match, *preceding;

    if (subject == NULL) return NULL;

    preceding = hap_hash_table_node_find(table, key, true);
    if (preceding == NULL) return NULL;

    if ((*preceding).key == key) {
        match = preceding;
        preceding = NULL;
    } else {
        match = (*preceding).next;
    }

    if (match == NULL) return NULL;

    if (preceding == NULL) {
        index = get_partition_index(table, key);
        (*(*subject).partitions)[index] = NULL;
    } else {
        (*preceding).next = (*match).next;
    }

    result = (*match).value;
    if (match != NULL) free(match);
    return result;
}


void* hap_hash_table_get(void *table, void *key) {
    HAPHashTableNode *node = hap_hash_table_node_find(table, key, false);

    if (node == NULL) return NULL;
    return (*node).value;
}

void* hap_hash_table_insert(void *table, void *key, void *value) {
    unsigned long index = get_partition_index(table, key);
    void *result = NULL;

    HAPHashTable *subject = (HAPHashTable*) table;
    HAPHashTableNode *match = NULL;
    HAPHashTableNode *previous = hap_hash_table_node_find(table, key, true);

    if (subject == NULL) return NULL;

    if (previous && (*previous).key == key) {
        match = previous;
        result = (*previous).value;
    } else if (previous && (*previous).next) {
        match = (*previous).next;
        result = (*match).value;
    } else {
        (*(*subject).partitions)[index] = hap_hash_table_node_create(key, value);
        match = (*(*subject).partitions)[index];
    }

    return result;
}

bool hap_hash_table_contains(void *table, void *key) {
    return hap_hash_table_find(table, key) != NULL;
}
