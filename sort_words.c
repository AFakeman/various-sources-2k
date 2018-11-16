#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int strcmp_void(const void *a, const void *b) {
    return -strcmp(* (const char**) a, * (const char**) b);
}

int main(int argc, char **argv) {
    FILE *input;
    FILE *output;
    char *words[50];
    char word_buffer[50 * 80 + 1];
    char *wb_iter;
    int word_idx;
    int last_space;
    int word_begin;
    if (argc != 3) {
        fprintf(stdout, "Usage: main <input> <output>");
        return -1;
    }
    input = fopen(argv[1], "r");
    if (!input) {
        fprintf(stderr, "Could not open file for reading\n");
        return -1;
    }
    output = fopen(argv[2], "w");
    if (!output) {
        fprintf(stderr, "Could not open file for writing\n");
        fclose(input);
        return -1;
    }
    while(fgets(word_buffer, 50 * 80, input)) {
        word_idx = 0;
        last_space = 0;
        word_begin = -1;
        for(wb_iter = word_buffer; *wb_iter; wb_iter++) {
            if (*wb_iter == ' ' && word_begin != -1) {
                *wb_iter = '\0';
                words[word_idx++] = word_buffer + word_begin;
                word_begin = -1;
            } else if (*wb_iter != ' ') {
                if (word_begin == -1) {
                    word_begin = wb_iter - word_buffer;
                }
            }
        }
        if (word_begin != -1) {
            words[word_idx++] = word_buffer + word_begin;
            wb_iter[-1] = '\0';
        }
        qsort(words, word_idx, sizeof(char*), strcmp_void);
        fprintf(stdout, "words read: %d\n", word_idx);
        for(; word_idx; word_idx--) {
            fprintf(stdout, "%s%c", words[word_idx - 1], word_idx == 1 ? '\n' : ' ');
            fprintf(output, "%s%c", words[word_idx - 1], word_idx == 1 ? '\n' : ' ');
        }
    }
    fclose(input);
    fclose(output);
    return 0;
}
