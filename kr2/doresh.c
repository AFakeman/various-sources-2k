#include <stdio.h>

int main() {
    int n, i;
    if (scanf("%d%d", &n, &i) != 2 || i < 0) {
        printf("Bad input\n");
        return -1;
    }
    if (i < 0 || i >= 8 * sizeof(n)) {
        printf("Invalid i value\n");
        return -1;
    }
    printf("%d\n", n | 1 << i);
}
