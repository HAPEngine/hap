#include <stdlib.h>

#include <hap/vector.h>


HAPVector* hap_vector_create(short size) {
    HAPVector *v = calloc(1, sizeof(HAPVector));
    if (v == NULL) return NULL;

    (*v).size = size;
    (*v).domains = calloc((*v).size, sizeof(HAPVector));

    if ((*v).domains == NULL) {
        free(v);
        v = NULL;
    }

    return v;
}


HAPVector* hap_vector_copy(HAPVector *v1) {
    if (v1 == NULL) return NULL;

    HAPVector *final = hap_vector_create((*v1).size);
    if (final == NULL) return NULL;

    for (short i = 0; i < (*v1).size; ++i) (*(*final).domains)[i] = (*(*v1).domains)[i];
    return final;
}


HAPVector* hap_vector_add(HAPVector *v1, HAPVector *v2) {
    HAPVector* final;

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


void hap_vector_destroy(HAPVector *v) {
    if (v == NULL) return;
    if ((*v).domains != NULL) free((*v).domains);
    free(v);
}
