#include <hap/typedefs.h>


struct HAPVector {
    short size;
    Unit (*domains)[];
};


HAPVector* hap_vector_add(HAPVector *v1, HAPVector *v2);
HAPVector* hap_vector_copy(HAPVector *v1);
HAPVector* hap_vector_create(short size);
