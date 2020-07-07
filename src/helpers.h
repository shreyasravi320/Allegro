#ifndef HELPERS_H
#define HELPERS_H

#include <iostream>
#include <vector>
#include "vectors.h"
#include "triangle.h"

using namespace std;

extern const double PI;

void int_swap(int* a, int* b);
void double_swap(double* a, double* b);
double deg_to_rad(double deg);
double rad_to_deg(double rad);
void merge_sort(vector<triangle_t>& v, int size);
void merge(vector<triangle_t>& parent, vector<triangle_t>& left, int left_size, vector<triangle_t>& right, int right_size);

#endif
