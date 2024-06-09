#include "memoryLib.h"
#include <stdio.h>
#include <stdbool.h>

node_t* head = NULL;
static int i = 1;

void* mem_allock(void* ptr, unsigned int bitSize) {
    node_t* node = (node_t*)malloc(sizeof(node_t));
    if (node == NULL) {
        printf("Failed to allocate memory for node.\n");
        return NULL;
    }

    node->next = NULL;
    node->size = bitSize;
    node->id = i;

    if (head == NULL) {
        if (ptr == NULL) {
            ptr = allock_new_memory(bitSize);
            node->addr = ptr;
        } else {
            printf("Address not found in the list...\n");
            free(node);
            return ptr;
        }
        head = node;
        node->id = 0;
        return head->addr;
    }

    node_t* current_node = head;
    bool isFound = false;

    if (ptr == NULL) {
        ptr = allock_new_memory(bitSize);
        node->addr = ptr;
    } else {
        if (current_node == head && current_node->addr == ptr) {
            printf("[HEAD] - Address found in the list with ID: %d\n", current_node->id);
            if (reallock_memory(current_node, bitSize) == E_OK) {
                free(node);
                return current_node->addr;
            } else {
                printf("reallock_memory error\n");
                free(node);
                return NULL;
            }
        }

        while (current_node != NULL) {
            if (current_node->addr == ptr) {
                printf("Address found in the list with ID: %d\n", current_node->id);
                if (reallock_memory(current_node, bitSize) == E_OK) {
                    isFound = true;
                    free(node);
                    return current_node->addr;
                } else {
                    printf("reallock_memory error\n");
                    free(node);
                    return NULL;
                }
            }
            current_node = current_node->next;
        }

        if (!isFound) {
            printf("Address not found in the list...\n");
            free(node);
            return ptr;
        }
    }

    current_node = head;
    while (current_node->next != NULL) {
        current_node = current_node->next;
    }

    current_node->next = node;
    i++;
    return node->addr;
}

int mem_free(void* ptr) {
    ErrorCode errorCode = E_NOT_KNOW_ADDR;
    node_t* current_node = head;
    node_t* prev_node = NULL;

    while (current_node != NULL) {
        if (current_node->addr == ptr) {
            if (prev_node == NULL) {
                head = current_node->next;
            } else {
                prev_node->next = current_node->next;
            }
            free(current_node->addr);
            free(current_node);
            errorCode = E_KNOW_ADDR;
            break;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }

    return errorCode;
}

void print_list() {
    node_t* current_node = head;
    if (head == NULL) {
        printf("The list is empty...\n");
    }
    while (current_node != NULL) {
        printf("ID: [%d]\n", current_node->id);
        current_node = current_node->next;
    }
}

ErrorCode reallock_memory(node_t* current, int bitSize) {
    ErrorCode errorCode = E_NOT_OK;

    if (current->size != bitSize) {
        printf("Changing memory size from [%d] to [%d]..\n", current->size, bitSize);
        void* new_addr = realloc(current->addr, bitSize);
        if (new_addr != NULL) {
            current->addr = new_addr;
            current->size = bitSize;
            errorCode = E_OK;
        }
    } else {
        printf("Sizes are the same, no need to reallocate\n");
        errorCode = E_OK;
    }

    return errorCode;
}

void* allock_new_memory(unsigned int bitSize) {
    printf("Allocating new memory...\n");
    void* ptr = malloc(bitSize);
    return ptr;
}
