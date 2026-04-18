#include "CgeMath.h"
#include <string.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

void CgeBox2fUnion(const float aMin[2], const float aMax[2],
                   const float bMin[2], const float bMax[2],
                   float outMin[2], float outMax[2]) {
    CgeVec2fMin(aMin, bMin, outMin);
    CgeVec2fMax(aMax, bMax, outMax);
}

int CgeBox2fIntersect(const float aMin[2], const float aMax[2],
                      const float bMin[2], const float bMax[2],
                      float outMin[2], float outMax[2]) {
    CgeVec2fMax(aMin, bMin, outMin);
    CgeVec2fMin(aMax, bMax, outMax);

    if (outMin[0] >= outMax[0] || outMin[1] >= outMax[1])
        return 0;

    return 1;
}

int CgeBox2fContains(const float aMin[2], const float aMax[2],
                     const float point[2]) {
    if (point[0] < aMin[0] || point[1] < aMin[1])
        return 0;

    if (point[0] > aMax[0] || point[1] > aMax[1])
        return 0;

    return 1;
}

int CgeBox2fEnclose(const float *points, size_t size, float outMin[2],
                    float outMax[2]) {
    float tmp1[2], tmp2[2];
    size_t i;

    if (!size)
        return 0;

    memcpy(tmp1, points, sizeof(tmp1));
    memcpy(tmp2, points, sizeof(tmp2));

    for (i = 1; i < size; i++) {
        CgeVec2fMin(tmp1, points + i * 2, tmp1);
        CgeVec2fMax(tmp2, points + i * 2, tmp2);
    }

    memcpy(outMin, tmp1, sizeof(tmp1));
    memcpy(outMax, tmp2, sizeof(tmp2));
    return 1;
}
