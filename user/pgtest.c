#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define ARRAY_SIZE 1024

int main() {
    int* array = malloc(ARRAY_SIZE * sizeof(int)); // Выделение памяти под массив
    int check;

    // Проверка, что без доступа к буферу статус равен 0
    pgaccess(array, sizeof(int) * ARRAY_SIZE, &check);
    if (check != 0) {
        printf("Test 1 failed\n");
        exit(1);
    }

    // Доступ к первому элементу буфера и проверка, что статус равен 1
    array[0] = 42;
    pgaccess(array, sizeof(int), &check);
    if (check != 1) {
        printf("Test 2 failed\n");
        exit(1);
    }

    // Проверка двойного вызова pgaccess: первый раз статус должен быть 1, второй раз 0
    array[256] = 84;
    pgaccess(array + 256, sizeof(int), &check);
    if (check != 1) {
        printf("Test 3 failed\n");
        exit(1);
    }
    pgaccess(array + 256, sizeof(int), &check);
    if (check != 0) {
        printf("Test 3 failed\n");
        exit(1);
    }

    // Проверка доступа к переменной на стеке
    int stack_var;
    pgaccess(&stack_var, sizeof(int), 0);
    stack_var = 128;
    pgaccess(&stack_var, sizeof(int), &check);
    if (check != 1) {
        printf("Test 4 failed\n");
        exit(1);
    }

    // Проверка двойного вызова pgaccess для переменной на стеке
    pgaccess(&stack_var, sizeof(int), 0);
    stack_var = 256;
    pgaccess(&stack_var, sizeof(int), &check);
    if (check != 1) {
        printf("Test 3 failed\n");
        exit(1);
    }
    pgaccess(&stack_var, sizeof(int), &check);
    if (check != 0) {
        printf("Test 5 failed\n");
        exit(1);
    }

    printf("All tests passed!!!\n");
    free(array);
    exit(0);
}
