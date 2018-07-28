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


