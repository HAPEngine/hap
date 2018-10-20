#include <hap/typedefs.h>


struct Vector {
    short size;
    Unit (*domains)[];
};


Vector* hap_vector_add(Vector *v1, Vector *v2);
Vector* hap_vector_copy(Vector *v1);
Vector* hap_vector_create(short size);
