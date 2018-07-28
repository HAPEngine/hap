#include <string.h>
#include <stdbool.h>

#include <hap.h>

#include "hashes.h"


// TODO: This could be dynamic, or maybe we could store in a trie?
#define SYMBOLS_BUCKET_SIZE 1024


typedef struct HAPSymbolNode HAPSymbolNode;


struct HAPSymbolNode {
   HAPSymbol      symbol;
   HAPSymbolNode  *next;
};


HAPSymbolNode *symbols[SYMBOLS_BUCKET_SIZE];


HAPSymbolNode* symbol_node_create(char * const name) {
   // Create a node with the given name

   HAPSymbolNode *node = calloc(1, sizeof(HAPSymbolNode));
   if (node == NULL) return NULL;

   (*node).symbol.name = calloc(strlen(name), sizeof(char));

   if ((*node).symbol.name == NULL) {
      free(node);
      return NULL;
   }

   strcpy((*node).symbol.name, name);

   return node;
}

HAPSymbol* symbol_find(char * const name, bool insert) {
   // Get a reference to a symbol with the given name. If the given name is not
   // already a symbol, one will be created.

   HAPSymbolNode *node = NULL, *prev = NULL;
   HAPSymbol *result = NULL;

   unsigned long index = hash_str(name) % (SYMBOLS_BUCKET_SIZE - 1);

   if (symbols[index] == NULL) {
      if (insert != true) return NULL;
      symbols[index] = node = symbol_node_create(name);

   } else {
      node = symbols[index];

      while (strcmp((*node).symbol.name, name) != 0) {
         prev = node;
         node = (*node).next;
      }

      if (node == NULL && insert == true)
         prev->next = node = symbol_node_create(name);
   }

   // Failed to get or create a node, so we return a failure state :'(
   if (node == NULL) return NULL;

   result = &(*node).symbol;
   ++((*result).count);

   return result;
}

void symbol_free(HAPSymbol *symbol) {
   // Free memory used by a symbol

   if ((*symbol).name == NULL) free((*symbol).name);
}

const HAPSymbol* hap_symbol_get(char * const name) {
   // Get a reference to a symbol with the given name

   return symbol_find(name, true);
}

unsigned int hap_symbol_release(char * const name) {
   // Releases a symbol by name from one of it's references. If the symbol has no
   // more references, it is removed from the buckets and freed from memory.
   // The number of remaining references to this symbol is returned. It is
   // expected that a user of a symbol will not reference a symbol once it has
   // released it, since there is no guarantee that it still exists any more.

   unsigned long index = 0;

   HAPSymbolNode *node = NULL, *prev = NULL;
   HAPSymbol *symbol = symbol_find(name, false);

   if (symbol == NULL) return 0;

   --(*symbol).count;

   if ((*symbol).count == 0) {
      index = hash_str(name) % (SYMBOLS_BUCKET_SIZE - 1);

      node = symbols[index];
      
      // This should never happen, but 
      if (node == NULL) {
         fprintf(stderr, "A corrupt node was encountered while releasing symbol: %s\n", name);
         return 0;
      }

      while (&(*node).symbol != symbol) {
         prev = node;
         node = (*node).next;
      }

      if (prev != NULL && node != NULL)
         (*prev).next = (*node).next;

      symbol_free(symbol);
      free(node);

      return 0;
   }

   return (*symbol).count;
}
