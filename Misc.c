#include "CgeMath.h"

float CgeLerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

void CgeTriangle3fBarycentric(const float a[3], const float b[3],
                              const float c[3], const float point[3],
                              float out[3]) {
    float tmp1[3], tmp2[3], tmp3[3];
    float t11, t12, t22, t31, t32, denom;

    CgeVec3fSub(b, a, tmp1);
    CgeVec3fSub(c, a, tmp2);
    CgeVec3fSub(point, a, tmp3);

    t11 = CgeVec3fDot(tmp1, tmp1);
    t12 = CgeVec3fDot(tmp1, tmp2);
    t22 = CgeVec3fDot(tmp2, tmp2);
    t31 = CgeVec3fDot(tmp3, tmp1);
    t32 = CgeVec3fDot(tmp3, tmp2);
    denom = 1.0f / (t11 * t22 - t12 * t12);

    out[1] = (t22 * t31 - t12 * t32) * denom;
    out[2] = (t11 * t32 - t12 * t31) * denom;
    out[0] = 1.0f - out[1] - out[2];
}
