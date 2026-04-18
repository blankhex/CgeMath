#include "CgeMath.h"

void CgeVec2iAdd(const int a[2], const int b[2], int out[2]) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
}

void CgeVec2iSub(const int a[2], const int b[2], int out[2]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
}

void CgeVec2iMul(const int a[2], const int b[2], int out[2]) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
}

void CgeVec2iScale(const int a[2], int b, int out[2]) {
    out[0] = a[0] * b;
    out[1] = a[1] * b;
}

void CgeVec2iMulAdd(const int a[2], const int b[2], const int c[2],
                    int out[2]) {
    out[0] = a[0] * b[0] + c[0];
    out[1] = a[1] * b[1] + c[1];
}

void CgeVec2iNegate(const int in[2], int out[2]) {
    out[0] = -in[0];
    out[1] = -in[1];
}

void CgeVec2iMin(const int a[2], const int b[2], int out[2]) {
    if (a[0] < b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] < b[1]) out[1] = a[1]; else out[1] = b[1];
}

void CgeVec2iMax(const int a[2], const int b[2], int out[2]) {
    if (a[0] > b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] > b[1]) out[1] = a[1]; else out[1] = b[1];
}
