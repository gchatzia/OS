#include <stdio.h>
#include <stdlib.h>

int add(int* a, int* b, int* c) {
    return *a + *b + *c;
}

int main(int arc, char *argv[]) {
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    int num3 = atoi(argv[3]);

    printf("Sum result: %d\n", add(&num1, &num2, &num3));
    return 0;
}
