#include "CgeMath.h"
#include <string.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

int CgePlaneFromPoints(const float a[3], const float b[3], const float c[3],
                       float out[4]) {
    float tmp1[3], tmp2[3];

    CgeVec3fSub(b, a, tmp1);
    CgeVec3fSub(c, a, tmp2);
    CgeVec3fCross(tmp2, tmp1, tmp1);
    if (CgeVec3fNormalEx(tmp1, tmp1) < EPSILON)
        return 0;

    out[3] = CgeVec3fDot(a, tmp1);
    memcpy(out, tmp1, sizeof(tmp1));
    return 1;
}

float CgePlaneDistance(const float plane[4], const float point[3]) {
    return CgeVec3fDot(plane, point) - plane[3];
}

void CgePlaneClosestPoint(const float plane[4], const float point[3],
                          float out[3]) {
    float tmp[3];

    CgeVec3fScale(plane, CgePlaneDistance(plane, point), tmp);
    CgeVec3fSub(point, tmp, out);
}
