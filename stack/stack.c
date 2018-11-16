#include "stack.h"
#include "stdio.h"

void af_stack_push(Stack *st, int value, AF_STACK_ERR *err) {
    if (!st) {
        *err = AF_STACK_NULL;
        return;
    }
    if (st->head < st->cap) {
        st->array[st->head++] = value;
        *err = AF_STACK_SUCCESS;
        return;
    } else {
        *err = AF_STACK_FULL;
        return;
    }
}

int af_stack_pop(Stack *st, AF_STACK_ERR *err) {
    if (!st) {
        *err = AF_STACK_NULL;
        return 0;
    }
    if (!st || !st->head) {
        *err = AF_STACK_EMPTY;
        return 0;
    };
    *err = AF_STACK_SUCCESS;
    return st->array[--(st->head)];
}

Stack *af_stack_create(int size, AF_STACK_ERR *err) {
    Stack *new_st;
    if(new_st = (Stack*) malloc(sizeof(Stack))) {
        new_st->head = 0;
        new_st->cap = size;
        if (!(new_st->array = (int*) malloc(sizeof(int) * size))) {
            free(new_st);
            new_st = NULL;
            *err = AF_STACK_BAD_MALLOC;
        } else {
            *err = AF_STACK_SUCCESS;
        }
    } else {
        *err = AF_STACK_BAD_MALLOC;
        fprintf(stderr, "Could not allocate memory for a stack object");
    }
    return new_st;
}

void af_stack_delete(Stack *st, AF_STACK_ERR *err) {
    if (!st) {
        *err = AF_STACK_SUCCESS;
        return;
    }
    free(st->array);
    free(st);
}
