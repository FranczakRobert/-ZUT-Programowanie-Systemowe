#include "memoryLib.h"
#include <stdio.h>

#define SIZE(x) x * sizeof(int)

int main() {
    int *test1 = NULL;
    int *test2 = NULL;
    int *test3 = (int*) malloc(sizeof(int) * 2);
    int *test4 = NULL;

    test1 = mem_allock(test1, SIZE(3));
    test2 = mem_allock(test2, SIZE(3));
    test4 = mem_allock(test4, SIZE(3));
    test2 = mem_allock(test2, SIZE(8));

    mem_free(test1);
    mem_free(test2);
    mem_free(test4);

    free(test3);
    print_list();

    return 0;
}
