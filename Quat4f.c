#include "CgeMath.h"
#include <string.h>
#include <math.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

void CgeQuat4fIdentity(float out[4]) {
    static const float ident[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    memcpy(out, ident, sizeof(ident));
}

void CgeQuat4fConjugate(const float in[4], float out[4]) {
    out[0] = -in[0];
    out[1] = -in[1];
    out[2] = -in[2];
    out[3] = in[3];
}

void CgeQuat4fInverse(const float in[4], float out[4]) {
    float dot;

    dot = CgeVec4fDot(in, in);
    CgeQuat4fConjugate(in, out);
    CgeQuat4fScale(out, 1.0f / dot, out);
}

void CgeQuat4fMul(const float a[4], const float b[4], float out[4]) {
    float tmp1[4], tmp2[4], tmp3[4];
    float w;

    w = a[3] * b[3] - CgeVec3fDot(a, b);
    CgeVec4fScale(a, b[3], tmp1);
    CgeVec4fScale(b, a[3], tmp2);
    CgeVec3fCross(a, b, tmp3);
    CgeVec4fAdd(tmp1, tmp2, out);
    CgeVec4fAdd(tmp3, out, out);
    out[3] = w;
}

void CgeQuat4fSlerp(const float a[4], const float b[4], float t, float out[4]) {
    float angle, denom;
    float from[4], to[4];

    angle = acosf(CgeVec4fDot(a, b));
    if (fabsf(angle) < EPSILON) {
        CgeVec4fLerp(a, b, t, out);
        return;
    }

    denom = 1.0f / sinf(angle);
    CgeVec4fScale(a, sinf((1 - t) * angle) * denom, from);
    CgeVec4fScale(b, sinf(t * angle) * denom, to);
    CgeVec4fAdd(from, to, out);
}

void CgeQuat4fFromEuler(float roll, float pitch, float yaw, float out[4]) {
    float cr, cp, cy, sr, sp, sy;

    cr = cosf(roll / 2.0f);
    cp = cosf(pitch / 2.0f);
    cy = cosf(yaw / 2.0f);
    sr = sinf(roll / 2.0f);
    sp = sinf(pitch / 2.0f);
    sy = sinf(yaw / 2.0f);

    out[0] = sr * cp * cy - cr * sp * sy;
    out[1] = cr * sp * cy + sr * cp * sy;
    out[2] = cr * cp * sy - sr * sp * cy;
    out[3] = cr * cp * cy + sr * sp * sy;
}

void CgeQuat4fFromAxis(const float axis[3], float angle, float out[4]) {
    float c, s;

    c = cosf(angle / 2.0f);
    s = sinf(angle / 2.0f);

    out[0] = axis[0] * s;
    out[1] = axis[1] * s;
    out[2] = axis[2] * s;
    out[3] = c;
}

void CgeQuat4fToEuler(const float in[4], float *roll, float *pitch,
                      float *yaw) {
    float ww, xw, yw, zw, xx, xy, xz, yy, yz, zz, angle;

    xx = in[0] * in[0];
    xy = in[0] * in[1];
    xz = in[0] * in[2];
    xw = in[0] * in[3];
    yy = in[1] * in[1];
    yz = in[1] * in[2];
    yw = in[1] * in[3];
    zz = in[2] * in[2];
    zw = in[2] * in[3];
    ww = in[3] * in[3];

    angle = 2.0f * (yw - xz);
    if (angle > 1.0f)
        angle = 1.0f;
    if (angle < -1.0f)
        angle = -1.0f;

    *pitch = asinf(angle);

    if (fabsf(*pitch - (PI / 2.0f)) < EPSILON) {
        *roll = 0.0f;
        *yaw = -2.0f * atan2f(in[0], in[3]);
    } else if (fabsf(*pitch - (PI / -2.0f)) < EPSILON) {
        *roll = 0.0f;
        *yaw = 2.0f * atan2f(in[0], in[3]);
    } else {
        *roll = atan2f(2.0f * (xw + yz), ww - xx - yy + zz);
        *yaw = atan2f(2.0f * (zw + xy), ww + xx - yy - zz);
    }
}

void CgeQuat4fToAxis(const float in[4], float axis[3], float *angle) {
    *angle = 2.0f * acosf(in[3]);

    if (fabsf(*angle) < EPSILON) {
        axis[0] = 1.0f;
        axis[1] = 0.0f;
        axis[2] = 0.0f;
    } else {
        float tmp;

        tmp = sqrtf(1.0f - in[3] * in[3]);
        axis[0] = in[0] / tmp;
        axis[1] = in[1] / tmp;
        axis[2] = in[2] / tmp;
    }
}

void CgeQuat4fToMat4f(const float in[4], float out[16]) {
    float xx, xy, xz, xw, yy, yz, yw, zz, zw;

    xx = in[0] * in[0];
    xy = in[0] * in[1];
    xz = in[0] * in[2];
    xw = in[0] * in[3];
    yy = in[1] * in[1];
    yz = in[1] * in[2];
    yw = in[1] * in[3];
    zz = in[2] * in[2];
    zw = in[2] * in[3];

    CgeMat4fIdentity(out);
    out[0] = 1.0f - 2.0f * (yy + zz);
    out[1] = 2.0f * (xy + zw);
    out[2] = 2.0f * (xz - yw);
    out[4] = 2.0f * (xy - zw);
    out[5] = 1.0f - 2.0f * (xx + zz);
    out[6] = 2.0f * (yz + xw);
    out[8] = 2.0f * (xz + yw);
    out[9] = 2.0f * (yz - xw);
    out[10] = 1.0f - 2.0f * (xx + yy);
}
