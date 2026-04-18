#include "CgeMath.h"
#include <string.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

void CgeBox3fUnion(const float aMin[3], const float aMax[3],
                   const float bMin[3], const float bMax[3],
                   float outMin[3], float outMax[3]) {
    CgeVec3fMin(aMin, bMin, outMin);
    CgeVec3fMax(aMax, bMax, outMax);
}

int CgeBox3fIntersect(const float aMin[3], const float aMax[3],
                      const float bMin[3], const float bMax[3],
                      float outMin[3], float outMax[3]) {
    CgeVec3fMax(aMin, bMin, outMin);
    CgeVec3fMin(aMax, bMax, outMax);

    if (outMin[0] >= outMax[0] || outMin[1] >= outMax[1] || outMin[2] >= outMax[2])
        return 0;

    return 1;
}

int CgeBox3fContains(const float aMin[3], const float aMax[3],
                     const float point[3]) {
    if (point[0] < aMin[0] || point[1] < aMin[1] || point[2] < aMin[2])
        return 0;

    if (point[0] > aMax[0] || point[1] > aMax[1] || point[2] > aMax[2])
        return 0;

    return 1;
}

int CgeBox3fEnclose(const float *points, size_t size, float outMin[3],
                    float outMax[3]) {
    float tmp1[3], tmp2[3];
    size_t i;

    if (!size)
        return 0;

    memcpy(tmp1, points, sizeof(tmp1));
    memcpy(tmp2, points, sizeof(tmp2));

    for (i = 1; i < size; i++) {
        CgeVec3fMin(tmp1, points + i * 3, tmp1);
        CgeVec3fMax(tmp2, points + i * 3, tmp2);
    }

    memcpy(outMin, tmp1, sizeof(tmp1));
    memcpy(outMax, tmp2, sizeof(tmp2));
    return 1;
}
