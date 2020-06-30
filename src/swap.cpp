#include "swap.h"

using namespace std;

void int_swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void double_swap(double* a, double* b) {
    double temp = *a;
    *a = *b;
    *b = temp;
}
