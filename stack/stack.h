#include <stdlib.h>

#ifndef STACK_AF
#define STACK_AF
typedef struct {
    int head;
    int cap;
    int *array;
} Stack;

typedef enum {
    AF_STACK_SUCCESS,
    AF_STACK_FULL,
    AF_STACK_BAD_MALLOC,
    AF_STACK_EMPTY,
    AF_STACK_NULL
} AF_STACK_ERR;

void af_stack_push(Stack *st, int value, AF_STACK_ERR *err);

int af_stack_pop(Stack *st, AF_STACK_ERR *err);

Stack *af_stack_create(int size, AF_STACK_ERR *err);

void af_stack_delete(Stack *st, AF_STACK_ERR *err);
#endif
