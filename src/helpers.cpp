#include "helpers.h"

using namespace std;

const double PI = 3.141592653589793238462;

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

double deg_to_rad(double deg) {
    return deg * PI / 180;
}

double rad_to_deg(double rad) {
    return rad * 180 / PI;
}

// Merge sort algorithm for depth sorting

void merge(vector<triangle_t>& parent, vector<triangle_t>& left, int left_size, vector<triangle_t>& right, int right_size) {
    int parent_idx = 0, left_idx = 0, right_idx = 0;

    while(left_idx < left_size && right_idx < right_size) {
        if(left[left_idx].avg_depth <= right[right_idx].avg_depth) {
            parent[parent_idx++] = left[left_idx++];
        }
        else {
            parent[parent_idx++] = right[right_idx++];
        }
    }
    while(left_idx < left_size) {
        parent[parent_idx++] = left[left_idx++];
    }
    while(right_idx < right_size) {
        parent[parent_idx++] = right[right_idx++];
    }
}

void merge_sort(vector<triangle_t>& v, int size) {

    if(size < 2) {
        return;
    }

    int left_size = size / 2;
    int right_size = size - left_size;

    vector<triangle_t> left;
    vector<triangle_t> right;
    for(int i = 0; i < left_size; ++i) {
        left.push_back(v[i]);
    }
    for(int i = 0; i < right_size; ++i) {
        right.push_back(v[i + left_size]);
    }

    merge_sort(left, left_size);
    merge_sort(right, right_size);

    merge(v, left, left_size, right, right_size);

    left.clear();
    left.shrink_to_fit();
    right.clear();
    right.shrink_to_fit();
}
