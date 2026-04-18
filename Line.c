#include "CgeMath.h"
#include <string.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

int CgeLineFromPoints(const float a[2], const float b[2], float out[3]) {
    float tmp[2];

    tmp[0] = a[1] - b[1];
    tmp[1] = b[0] - a[0];
    if (CgeVec2fNormalEx(tmp, tmp) < EPSILON)
        return 0;

    out[2] = CgeVec2fDot(tmp, a);
    memcpy(out, tmp, sizeof(tmp));
    return 1;
}

float CgeLineDistance(const float line[3], const float point[2]) {
    return CgeVec2fDot(line, point) - line[2];
}

void CgeLineClosestPoint(const float line[3], const float point[2],
                         float out[2]) {
    float tmp[2];

    CgeVec2fScale(line, CgeLineDistance(line, point), tmp);
    CgeVec2fSub(point, tmp, out);
}
