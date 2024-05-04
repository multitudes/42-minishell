#include <stdio.h>

void print_bytes(void *ptr, size_t size) {
    unsigned char *bytes = ptr;
    for (size_t i = 0; i < size; i++) {
        printf("%02x ", bytes[i]);
    }
    printf("\n");
}

/*
prints the memory representation of a 32-bit and 64-bit integer
32-bit integer in memory:
78 56 34 12 
64-bit integer in memory:
f0 de bc 9a 78 56 34 12 
*/
int main() {
    int x = 0x12345678;  // 32-bit integer
    long long y = 0xaabbccddeeff0011LL;  // 64-bit integer

    printf("32-bit integer in memory:\n");
    print_bytes(&x, sizeof(x));

    printf("64-bit integer in memory:\n");
    print_bytes(&y, sizeof(y));

    return 0;
}