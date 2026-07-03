#include "CgeMath.h"

void CgeVec3iAdd(const int a[3], const int b[3], int out[3]) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

void CgeVec3iSub(const int a[3], const int b[3], int out[3]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

void CgeVec3iMul(const int a[3], const int b[3], int out[3]) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
}

void CgeVec3iScale(const int a[3], int b, int out[3]) {
    out[0] = a[0] * b;
    out[1] = a[1] * b;
    out[2] = a[2] * b;
}

void CgeVec3iMulAdd(const int a[3], const int b[3], const int c[3],
                    int out[3]) {
    out[0] = a[0] * b[0] + c[0];
    out[1] = a[1] * b[1] + c[1];
    out[2] = a[2] * b[2] + c[2];
}

void CgeVec3iNegate(const int in[3], int out[3]) {
    out[0] = -in[0];
    out[1] = -in[1];
    out[2] = -in[2];
}

void CgeVec3iMin(const int a[3], const int b[3], int out[3]) {
    if (a[0] < b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] < b[1]) out[1] = a[1]; else out[1] = b[1];
    if (a[2] < b[2]) out[2] = a[2]; else out[2] = b[2];
}

void CgeVec3iMax(const int a[3], const int b[3], int out[3]) {
    if (a[0] > b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] > b[1]) out[1] = a[1]; else out[1] = b[1];
    if (a[2] > b[2]) out[2] = a[2]; else out[2] = b[2];
}

void CgeVec3iClamp(const int in[3], int minVal, int maxVal, int out[3]) {
    out[0] = in[0] < minVal ? minVal : (in[0] > maxVal ? maxVal : in[0]);
    out[1] = in[1] < minVal ? minVal : (in[1] > maxVal ? maxVal : in[1]);
    out[2] = in[2] < minVal ? minVal : (in[2] > maxVal ? maxVal : in[2]);
}

void CgeVec3iAbs(const int in[3], int out[3]) {
    out[0] = in[0] < 0 ? -in[0] : in[0];
    out[1] = in[1] < 0 ? -in[1] : in[1];
    out[2] = in[2] < 0 ? -in[2] : in[2];
}
