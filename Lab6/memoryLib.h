#include <stdlib.h>
#include <stdio.h>
#ifndef MEMORYLIB_H
#define MEMORYLIB_H

typedef enum ERROR_CODE{
    E_NOT_KNOW_ADDR = -1,
    E_KNOW_ADDR = 0,
    E_OK = 1,
    E_NOT_OK = 2
}ErrorCode;

typedef struct Node {
    struct Node* next;
    void* addr;
    int size;
    int id;
}node_t;

void* mem_allock(void* ptr, unsigned int size);
int mem_free(void* ptr);
ErrorCode reallock_memory(node_t * current, int biteSize);
void* allock_new_memory(unsigned int bitSize);
void print_list();

#endif /*MEMORYLIB_H*/