#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

int main() {
    AF_STACK_ERR err; 
    Stack *vitya = af_stack_create(255, &err); //tipo vitya st
    if (!vitya) {
        return 0;
    }
    af_stack_push(vitya, 10, &err);
    af_stack_push(vitya, 20, &err);
    af_stack_push(vitya, 30, &err);
    af_stack_push(vitya, 40, &err);
    for(;vitya->head;) {
        printf("%d\n", af_stack_pop(vitya, &err));
    }
    Stack *flow = af_stack_create(1, &err);
    af_stack_push(flow, 3, &err);
    af_stack_push(flow, 4, &err);
    printf("%d\n", af_stack_pop(flow, &err));
    printf("%d\n", af_stack_pop(flow, &err));
    af_stack_pop(vitya, &err);
    af_stack_push(NULL, 0, &err);
    af_stack_pop(NULL, &err);
    af_stack_delete(NULL, &err);
    af_stack_delete(vitya, &err);
    return 0;
}
