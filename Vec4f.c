#include "CgeMath.h"
#include <math.h>

#define SET_ROW(row, val) row[0] = row[1] = row[2] = row[3] = val

void CgeVec4fAdd(const float a[4], const float b[4], float out[4]) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
    out[3] = a[3] + b[3];
}

void CgeVec4fSub(const float a[4], const float b[4], float out[4]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
    out[3] = a[3] - b[3];
}

void CgeVec4fMul(const float a[4], const float b[4], float out[4]) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
    out[3] = a[3] * b[3];
}

void CgeVec4fScale(const float a[4], float b, float out[4]) {
    out[0] = a[0] * b;
    out[1] = a[1] * b;
    out[2] = a[2] * b;
    out[3] = a[3] * b;
}

void CgeVec4fMulAdd(const float a[4], const float b[4], const float c[4],
                    float out[4]) {
    out[0] = a[0] * b[0] + c[0];
    out[1] = a[1] * b[1] + c[1];
    out[2] = a[2] * b[2] + c[2];
    out[3] = a[3] * b[3] + c[3];
}

void CgeVec4fNegate(const float in[4], float out[4]) {
    out[0] = -in[0];
    out[1] = -in[1];
    out[2] = -in[2];
    out[3] = -in[3];
}

float CgeVec4fDot(const float a[4], const float b[4]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

float CgeVec4fLength(const float in[4]) {
    return sqrtf(CgeVec4fDot(in, in));
}

void CgeVec4fNormal(const float in[4],
                    float out[4]) {
    CgeVec4fScale(in, 1.0f / CgeVec4fLength(in), out);
}

float CgeVec4fNormalEx(const float in[4], float out[4]) {
    float length;

    length = CgeVec4fLength(in);
    CgeVec4fScale(in, 1.0f / length, out);
    return length;
}

void CgeVec4fMin(const float a[4], const float b[4], float out[4]) {
    if (a[0] < b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] < b[1]) out[1] = a[1]; else out[1] = b[1];
    if (a[2] < b[2]) out[2] = a[2]; else out[2] = b[2];
    if (a[3] < b[3]) out[3] = a[3]; else out[3] = b[3];
}

void CgeVec4fMax(const float a[4], const float b[4], float out[4]) {
    if (a[0] > b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] > b[1]) out[1] = a[1]; else out[1] = b[1];
    if (a[2] > b[2]) out[2] = a[2]; else out[2] = b[2];
    if (a[3] > b[3]) out[3] = a[3]; else out[3] = b[3];
}

void CgeVec4fLerp(const float a[4], const float b[4], float t, float out[4]) {
    float tmp[4];

    CgeVec4fSub(b, a, tmp);
    CgeVec4fScale(tmp, t, tmp);
    CgeVec4fAdd(a, tmp, out);
}

void CgeVec4fProject(const float a[4], const float b[4], float out[4]) {
    float amount;

    amount = CgeVec4fDot(a, b) / CgeVec4fDot(b, b);
    CgeVec4fScale(b, amount, out);
}

void CgeVec4fBarycentric(const float a[4], const float b[4], const float c[4],
                         float v, float w, float out[4]) {
    float tmp1[4], tmp2[4];
    float u;

    u = 1.0f - v - w;
    SET_ROW(tmp1, u); CgeVec4fMul(a, tmp1, tmp2);
    SET_ROW(tmp1, v); CgeVec4fMulAdd(b, tmp1, tmp2, tmp2);
    SET_ROW(tmp1, w); CgeVec4fMulAdd(c, tmp1, tmp2, out);
}

float CgeVec4fDistance(const float a[4], const float b[4]) {
    float tmp[4];

    CgeVec4fSub(a, b, tmp);
    return CgeVec4fLength(tmp);
}

void CgeVec4fClamp(const float in[4], float minVal, float maxVal, float out[4]) {
    out[0] = in[0] < minVal ? minVal : (in[0] > maxVal ? maxVal : in[0]);
    out[1] = in[1] < minVal ? minVal : (in[1] > maxVal ? maxVal : in[1]);
    out[2] = in[2] < minVal ? minVal : (in[2] > maxVal ? maxVal : in[2]);
    out[3] = in[3] < minVal ? minVal : (in[3] > maxVal ? maxVal : in[3]);
}

void CgeVec4fAbs(const float in[4], float out[4]) {
    out[0] = in[0] < 0 ? -in[0] : in[0];
    out[1] = in[1] < 0 ? -in[1] : in[1];
    out[2] = in[2] < 0 ? -in[2] : in[2];
    out[3] = in[3] < 0 ? -in[3] : in[3];
}
