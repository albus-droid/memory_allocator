#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void test_malloc_free() {
    printf("Testing malloc and free...\n");
    char *str = (char*)malloc(20);
    if (str == NULL) {
        printf("malloc failed\n");
        return;
    }
    strcpy(str, "Hello, World!");
    printf("%s\n", str);
    free(str);
    printf("malloc and free passed\n\n");
}

void test_calloc() {
    printf("Testing calloc...\n");
    int *arr = (int*)calloc(5, sizeof(int));
    if (arr == NULL) {
        printf("calloc failed\n");
        return;
    }
    for (int i = 0; i < 5; i++) {
        if (arr[i] != 0) {
            printf("calloc failed: memory not zeroed\n");
            free(arr);
            return;
        }
        arr[i] = i * i; // Initialize with some data
    }
    for (int i = 0; i < 5; i++) {
        printf("%d ", arr[i]);
    }
    printf("\ncalloc passed\n\n");
    free(arr);
}

void test_realloc() {
    printf("Testing realloc...\n");
    int *arr = (int*)malloc(5 * sizeof(int));
    if (arr == NULL) {
        printf("malloc failed\n");
        return;
    }
    for (int i = 0; i < 5; i++) {
        arr[i] = i;
    }
    int *resized_arr = (int*)realloc(arr, 10 * sizeof(int));
    if (resized_arr == NULL) {
        printf("realloc failed\n");
        free(arr); // Original array still needs to be freed on failure
        return;
    }
    arr = resized_arr; // Use the potentially new pointer
    for (int i = 5; i < 10; i++) {
        arr[i] = i * i; // Initialize additional space
    }
    for (int i = 0; i < 10; i++) {
        printf("%d ", arr[i]);
    }
    printf("\nrealloc passed\n\n");
    free(arr);
}

int main() {
    test_malloc_free();
    test_calloc();
    test_realloc();
    return 0;
}

