#include <hap.h>

const void* hap_hash_table_create(unsigned long size, bool fixed);
void hap_hash_table_destroy(void *table);

bool hap_hash_table_contains(void *table, void *key);
void* hap_hash_table_find(void *table, void *key, bool getPreceding);
void* hap_hash_table_remove(void *table, void *key);
void* hap_hash_table_insert(void *table, void *key, void *value);
