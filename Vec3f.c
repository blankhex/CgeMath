#include "CgeMath.h"
#include <string.h>
#include <math.h>

#define SET_ROW(row, val) row[0] = row[1] = row[2] = val

void CgeVec3fAdd(const float a[3], const float b[3], float out[3]) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
}

void CgeVec3fSub(const float a[3], const float b[3], float out[3]) {
    out[0] = a[0] - b[0];
    out[1] = a[1] - b[1];
    out[2] = a[2] - b[2];
}

void CgeVec3fMul(const float a[3], const float b[3], float out[3]) {
    out[0] = a[0] * b[0];
    out[1] = a[1] * b[1];
    out[2] = a[2] * b[2];
}

void CgeVec3fScale(const float a[3], float b, float out[3]) {
    out[0] = a[0] * b;
    out[1] = a[1] * b;
    out[2] = a[2] * b;
}

void CgeVec3fMulAdd(const float a[3], const float b[3], const float c[3],
                    float out[3]) {
    out[0] = a[0] * b[0] + c[0];
    out[1] = a[1] * b[1] + c[1];
    out[2] = a[2] * b[2] + c[2];
}

void CgeVec3fNegate(const float in[3], float out[3]) {
    out[0] = -in[0];
    out[1] = -in[1];
    out[2] = -in[2];
}

float CgeVec3fDot(const float a[3], const float b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void CgeVec3fCross(const float a[3], const float b[3], float out[3]) {
    float tmp[3];

    tmp[0] = a[1] * b[2] - a[2] * b[1];
    tmp[1] = a[2] * b[0] - a[0] * b[2];
    tmp[2] = a[0] * b[1] - a[1] * b[0];
    memcpy(out, tmp, sizeof(tmp));
}

float CgeVec3fLength(const float in[3]) {
    return sqrtf(CgeVec3fDot(in, in));
}

void CgeVec3fNormal(const float in[3], float out[3]) {
    CgeVec3fScale(in, 1.0f / CgeVec3fLength(in), out);
}

float CgeVec3fNormalEx(const float in[3], float out[3]) {
    float length;

    length = CgeVec3fLength(in);
    CgeVec3fScale(in, 1.0f / length, out);
    return length;
}

void CgeVec3fMin(const float a[3], const float b[3], float out[3]) {
    if (a[0] < b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] < b[1]) out[1] = a[1]; else out[1] = b[1];
    if (a[2] < b[2]) out[2] = a[2]; else out[2] = b[2];
}

void CgeVec3fMax(const float a[3], const float b[3], float out[3]) {
    if (a[0] > b[0]) out[0] = a[0]; else out[0] = b[0];
    if (a[1] > b[1]) out[1] = a[1]; else out[1] = b[1];
    if (a[2] > b[2]) out[2] = a[2]; else out[2] = b[2];
}

void CgeVec3fLerp(const float a[3], const float b[3], float t, float out[3]) {
    float tmp[3];

    CgeVec3fSub(b, a, tmp);
    CgeVec3fScale(tmp, t, tmp);
    CgeVec3fAdd(a, tmp, out);
}

void CgeVec3fProject(const float a[3], const float b[3], float out[3]) {
    float amount;

    amount = CgeVec3fDot(a, b) / CgeVec3fDot(b, b);
    CgeVec3fScale(b, amount, out);
}

void CgeVec3fBarycentric(const float a[3], const float b[3], const float c[3],
                         float v, float w, float out[3]) {
    float tmp1[3], tmp2[3];
    float u;

    u = 1.0f - v - w;
    SET_ROW(tmp1, u); CgeVec3fMul(a, tmp1, tmp2);
    SET_ROW(tmp1, v); CgeVec3fMulAdd(b, tmp1, tmp2, tmp2);
    SET_ROW(tmp1, w); CgeVec3fMulAdd(c, tmp1, tmp2, out);
}

float CgeVec3fDistance(const float a[3], const float b[3]) {
    float tmp[3];

    CgeVec3fSub(a, b, tmp);
    return CgeVec3fLength(tmp);
}

void CgeVec3fReflect(const float in[3], const float norm[3], float out[3]) {
    float tmp[3];
    float dot;

    dot = CgeVec3fDot(in, norm);
    CgeVec3fScale(norm, 2.0f * dot, tmp);
    CgeVec3fSub(in, tmp, out);
}

void CgeVec3fRefract(const float in[3], const float norm[3], float eta,
                     float out[3]) {
    float tmp1[3], tmp2[3];
    float nDotI, k;

    nDotI = CgeVec3fDot(norm, in);
    k = 1.0f - eta * eta * (1.0f - nDotI * nDotI);
    if (k < 0.0f) {
        out[0] = out[1] = out[2] = 0.0f;
        return;
    }

    CgeVec3fScale(norm, eta * nDotI + (float)sqrt(k), tmp1);
    CgeVec3fScale(in, eta, tmp2);
    CgeVec3fSub(tmp2, tmp1, out);
}

void CgeVec3fClamp(const float in[3], float minVal, float maxVal, float out[3]) {
    out[0] = in[0] < minVal ? minVal : (in[0] > maxVal ? maxVal : in[0]);
    out[1] = in[1] < minVal ? minVal : (in[1] > maxVal ? maxVal : in[1]);
    out[2] = in[2] < minVal ? minVal : (in[2] > maxVal ? maxVal : in[2]);
}

void CgeVec3fAbs(const float in[3], float out[3]) {
    out[0] = in[0] < 0 ? -in[0] : in[0];
    out[1] = in[1] < 0 ? -in[1] : in[1];
    out[2] = in[2] < 0 ? -in[2] : in[2];
}
