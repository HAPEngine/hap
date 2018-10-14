#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include <hap.h>

#include "hashes.h"


#define MAX_ERROR_LENGTH 255
#define TAGS_BUCKET_SIZE 4096


typedef struct HAPTagNode HAPTagNode;


struct HAPTagNode {
   HAPTag      tag;
   HAPTagNode  *next;
};


HAPTagNode *tags[TAGS_BUCKET_SIZE];


void tag_destroy(HAPTag *tag) {
   // Clean up memory used for a tag

   if ((*tag).name != NULL)
      free((*tag).name);
}


HAPTagNode* tag_node_create(char * name) {
  errno_t error = 0;
  char errorMessage[MAX_ERROR_LENGTH] = "\0";

   int length = strlen(name);

   HAPTagNode *node = calloc(1, sizeof(HAPTagNode));
   if (node == NULL) return NULL;

   (*node).tag.name = calloc(length, sizeof(char));

   if ((*node).tag.name == NULL) {
      free(node);
      return NULL;
   }

   if ((error = strcpy_s((*node).tag.name, length, name)) == 0) {
	   if ((error = strerror_s(errorMessage, MAX_ERROR_LENGTH, error)) != 0) {
		   fprintf(stderr, "Failed to create tag node: %s\n", name);
	   }

	   fprintf(stderr, "Failed to create tag node: %s\n", errorMessage);
   }

   return node;
}


void tag_node_destroy(HAPTagNode* node) {
   const HAPSymbol *symbol = hap_symbol_get((*node).tag.name);
   unsigned long index = (hash_ptr(symbol) * sizeof(unsigned long)) % TAGS_BUCKET_SIZE;

   HAPTagNode *prev = tags[index];

   while (prev != NULL && (*prev).next != node) {
      prev = (*prev).next;
   }

   if (prev == NULL) {
      tags[index] = (*node).next;
   } else {
      (*prev).next = (*node).next;
   }

   tag_destroy(&(*node).tag);
}


HAPTag* tag_find(char * name, bool insert) {
   // Find a tag matching the given name

   HAPTagNode *node = NULL, *prev = NULL;
   HAPTag *result = NULL;

   const HAPSymbol *symbol = hap_symbol_get(name);
   unsigned long index = (hash_ptr(symbol) * sizeof(unsigned long)) % TAGS_BUCKET_SIZE;

   if (tags[index] == NULL) {
      if (insert == false) return NULL;
      tags[index] = node = tag_node_create(name);
   } else {
      node = tags[index];

      while (node != NULL && strcmp((*node).tag.name, name) != 0) {
         prev = node;
         node = (*node).next;
      }

      if (node == NULL && insert == true)
         (*prev).next = node = tag_node_create(name);
   }

   if (node == NULL) return NULL;

   result = &(*node).tag;

   return result;
}


const HAPTag* hap_tag_get(char * const name) {
   // Find a tag and return a const pointer to it

   return tag_find(name, true);
}


bool hap_tag_add(char * const name, void *value) {
   // Add an item to the list of items with this tag

   return false;
}


bool hap_tag_remove(char * const name, void *value) {
   // Remove an item from the list of items with this tag

   return false;
}
