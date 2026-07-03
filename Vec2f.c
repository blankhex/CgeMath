#include "CgeMath.h"
#include <math.h>

#define SET_ROW(row, val) row[0] = row[1] = val

void CgeVec2fAdd(const float a[2], const float b[2], float out[2]) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
}

void CgeVec2fSub(const float a[2], const float b[2], float out[2]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
}

void CgeVec2fMul(const float a[2], const float b[2], float out[2]) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
}

void CgeVec2fScale(const float a[2], float b, float out[2]) {
    out[0] = a[0] * b;
    out[1] = a[1] * b;
}

void CgeVec2fMulAdd(const float a[2], const float b[2], const float c[2],
                    float out[2]) {
    out[0] = a[0] * b[0] + c[0];
    out[1] = a[1] * b[1] + c[1];
}

void CgeVec2fNegate(const float in[2], float out[2]) {
    out[0] = -in[0];
    out[1] = -in[1];
}

float CgeVec2fDot(const float a[2], const float b[2]) {
    return a[0] * b[0] + a[1] * b[1];
}

float CgeVec2fCross(const float a[2], const float b[2]) {
    return a[0] * b[1] - a[1] * b[0];
}

float CgeVec2fLength(const float in[2]) {
    return sqrtf(CgeVec2fDot(in, in));
}

void CgeVec2fNormal(const float in[2], float out[2]) {
    CgeVec2fScale(in, 1.0f / CgeVec2fLength(in), out);
}

float CgeVec2fNormalEx(const float in[2], float out[2]) {
    float length;

    length = CgeVec2fLength(in);
    CgeVec2fScale(in, 1.0f / length, out);
    return length;
}

void CgeVec2fMin(const float a[2], const float b[2], float out[2]) {
    if (a[0] < b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] < b[1]) out[1] = a[1]; else out[1] = b[1];
}

void CgeVec2fMax(const float a[2], const float b[2], float out[2]) {
    if (a[0] > b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] > b[1]) out[1] = a[1]; else out[1] = b[1];
}

void CgeVec2fLerp(const float a[2], const float b[2], float t, float out[2]) {
    float tmp[2];

    CgeVec2fSub(b, a, tmp);
    CgeVec2fScale(tmp, t, tmp);
    CgeVec2fAdd(a, tmp, out);
}

void CgeVec2fProject(const float a[2], const float b[2], float out[2]) {
    float amount;

    amount = CgeVec2fDot(a, b) / CgeVec2fDot(b, b);
    CgeVec2fScale(b, amount, out);
}

void CgeVec2fBarycentric(const float a[2], const float b[2], const float c[2],
                         float v, float w, float out[2]) {
    float tmp1[2], tmp2[2];
    float u;

    u = 1.0f - v - w;
    SET_ROW(tmp1, u); CgeVec2fMul(a, tmp1, tmp2);
    SET_ROW(tmp1, v); CgeVec2fMulAdd(b, tmp1, tmp2, tmp2);
    SET_ROW(tmp1, w); CgeVec2fMulAdd(c, tmp1, tmp2, out);
}

float CgeVec2fDistance(const float a[2], const float b[2]) {
    float tmp[2];

    CgeVec2fSub(a, b, tmp);
    return CgeVec2fLength(tmp);
}

void CgeVec2fReflect(const float in[2], const float norm[2], float out[2]) {
    float tmp[2];
    float dot;

    dot = CgeVec2fDot(in, norm);
    CgeVec2fScale(norm, 2.0f * dot, tmp);
    CgeVec2fSub(in, tmp, out);
}

void CgeVec2fRefract(const float in[2], const float norm[2], float eta,
                     float out[2]) {
    float tmp1[2], tmp2[2];
    float nDotI, k;

    nDotI = CgeVec2fDot(norm, in);
    k = 1.0f - eta * eta * (1.0f - nDotI * nDotI);
    if (k < 0.0f) {
        out[0] = out[1] = 0.0f;
        return;
    }

    CgeVec2fScale(norm, eta * nDotI + (float)sqrt(k), tmp1);
    CgeVec2fScale(in, eta, tmp2);
    CgeVec2fSub(tmp2, tmp1, out);
}

void CgeVec2fClamp(const float in[2], float minVal, float maxVal, float out[2]) {
    out[0] = in[0] < minVal ? minVal : (in[0] > maxVal ? maxVal : in[0]);
    out[1] = in[1] < minVal ? minVal : (in[1] > maxVal ? maxVal : in[1]);
}

void CgeVec2fAbs(const float in[2], float out[2]) {
    out[0] = in[0] < 0 ? -in[0] : in[0];
    out[1] = in[1] < 0 ? -in[1] : in[1];
}
