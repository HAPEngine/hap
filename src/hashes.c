unsigned long hash_str(const char *subject) {
    // NOTE: This is djb2 but it has been slightly modified so that passing in
    // unsigned char * isn't necessary. EG, we want to pass in
    // hash_str("example") - which is unsigned by default. We can probably
    // optimize this to a more reasonable alogrithm once we have a few games to
    // analyze the number of items in each bucket for.

    unsigned long result = 5381;
    int c;
    while ((c = *subject++)) result = ((result << 5) + result) + (c + 128);
    return result;
}


unsigned long hash_ptr(void *subject) {
    // A temporary / lazy / simple way to hash a pointer. This will be improved
    // later, but I just need something to test with for now.

    return ((unsigned long) subject) * sizeof(unsigned long);
}
