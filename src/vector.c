#include <stdlib.h>

#include <hap/vector.h>


Vector* hap_vector_create(short size) {
    Vector *v = calloc(1, sizeof(Vector));
    if (v == NULL) return NULL;

    (*v).size = size;
    (*v).domains = calloc((*v).size, sizeof(Vector));

    if ((*v).domains == NULL) {
        free(v);
        v = NULL;
    }

    return v;
}


Vector* hap_vector_copy(Vector *v1) {
    if (v1 == NULL) return NULL;

    Vector *final = hap_vector_create((*v1).size);
    if (final == NULL) return NULL;

    for (short i = 0; i < (*v1).size; ++i) (*(*final).domains)[i] = (*(*v1).domains)[i];
    return final;
}


Vector* hap_vector_add(Vector *v1, Vector *v2) {
    Vector* final;

    if (v2 == NULL) return v1;
    if (v1 == NULL) return v2;
    if ((*v1).size != (*v2).size) return NULL;

    final = hap_vector_create((*v1).size);
    if (final == NULL) return NULL;

    for (short i = 0; i < (*v1).size; ++i) {
        (*(*final).domains)[i] = (*(*v1).domains)[i] + (*(*v2).domains)[i];
    }

    return final;
}


void hap_vector_destroy(Vector *v) {
    if (v == NULL) return;
    if ((*v).domains != NULL) free((*v).domains);
    free(v);
}

