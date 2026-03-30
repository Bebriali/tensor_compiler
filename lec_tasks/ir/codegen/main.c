#include <stdio.h>

// Объявляем внешнюю функцию
extern int functional(int *a, int n);

// Исходная функция на Си для сравнения
int functional_c(int *a, int n) {
    int res = 0;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            res += a[i] * 2;
        } else {
            res -= (a[i] + 3);
        }
    }
    return res;
}

int main() {
    int array[] = {10, 20, 30, 40, 50};
    int n = 5;

    int res_asm = functional(array, n);
    int res_c = functional_c(array, n);

    printf("ASM Result: %d\n", res_asm);
    printf("C Result:   %d\n", res_c);

    if (res_asm == res_c) {
        printf("SUCCESS: Results match!\n");
    } else {
        printf("ERROR: Results differ!\n");
    }

    return 0;
}