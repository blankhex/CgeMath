#include "CgeMath.h"
#include <string.h>
#include <math.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

int CgeRay2fIntersectLine(const float start[2], const float direction[2],
                          const float line[3], float *t, float out[2]) {
    float tmp1[2];
    float denom, time;

    /* Calculate intersection time */
    denom = CgeVec2fDot(direction, line);
    time = (line[2] - CgeVec2fDot(line, start)) / denom;

    /* Check for ray/plane parallel to each other or point is behing the ray. */
    if (fabsf(denom) < EPSILON || time < 0.0f)
        return 0;

    /* Compute intersection point */
    CgeVec2fScale(direction, time, tmp1);
    CgeVec2fAdd(start, tmp1, out);
    *t = time;
    return 1;
}

int CgeRay2fIntersectTime(const float aStart[2], const float aDirection[2],
                          const float bStart[2], const float bDirection[2],
                          float *time1, float *time2) {
    float tmp1[2], tmp2[2], tmp3[2];
    float denom;

    /* Rotate directions by 90 degrees and caluclate denom */
    tmp1[0] = -aDirection[1]; tmp1[1] = aDirection[0];
    tmp2[0] = -bDirection[1]; tmp2[1] = bDirection[0];
    denom = CgeVec2fDot(tmp1, bDirection);

    if (fabsf(denom) < EPSILON)
        return 0;

    /* Calculate segments offset and intersection times */
    CgeVec2fSub(aStart, bStart, tmp3);
    *time1 = CgeVec2fDot(tmp3, tmp2) / denom;
    *time2 = CgeVec2fDot(tmp3, tmp1) / denom;

    return 1;
}

int CgeRay2fIntersectRay(const float aStart[2], const float aDirection[2],
                         const float bStart[2], const float bDirection[2],
                         float *t, float out[2]) {
    float tmp[2];
    float time1, time2;

    if (CgeRay2fIntersectTime(aStart, aDirection, bStart, bDirection, &time1, &time2))
        return 0;

    if (time1 < 0.0f || time2 < 0.0f)
        return 0;

    CgeVec2fScale(aDirection, time1, tmp);
    CgeVec2fAdd(aStart, tmp, out);
    *t = time1;
    return 1;
}

int CgeRay2fIntersectSegment(const float aStart[2], const float aDirection[2],
                             const float bStart[2], const float bEnd[2],
                             float *t, float out[2]) {
    float tmp[2];
    float time1, time2;

    CgeVec2fSub(bEnd, bStart, tmp);
    if (CgeRay2fIntersectTime(aStart, aDirection, bStart, tmp, &time1, &time2))
        return 0;

    if (time1 < 0.0f || time2 < 0.0f || time2 > 1.0f)
        return 0;

    CgeVec2fScale(aDirection, time1, tmp);
    CgeVec2fAdd(aStart, tmp, out);
    *t = time1;
    return 1;
}

int CgeSegment2fIntersectLine(const float start[2], const float end[2],
                              const float line[3], float *t, float out[2]) {
    float tmp[2];
    float denom, time;

    /* Calculate intersection time */
    CgeVec2fSub(end, start, tmp);
    denom = CgeVec2fDot(tmp, line);
    time = (line[2] - CgeVec2fDot(line, start)) / denom;

    /* Check for ray/plane parallel to each other or point is behing the ray. */
    if (fabsf(denom) < EPSILON || time < 0.0f || time > 1.0f)
        return 0;

    /* Compute intersection point */
    CgeVec2fScale(tmp, time, tmp);
    CgeVec2fAdd(start, tmp, out);
    *t = time;
    return 1;
}

int CgeSegment2fIntersectSegment(const float aStart[2], const float aEnd[2],
                                 const float bStart[2], const float bEnd[2],
                                 float *t, float out[2]) {
    float tmp1[2], tmp2[2];
    float time1, time2;

    CgeVec2fSub(aEnd, aStart, tmp1);
    CgeVec2fSub(bEnd, bStart, tmp2);
    if (CgeRay2fIntersectTime(aStart, tmp1, bStart, tmp2, &time1, &time2))
        return 0;

    if (time1 < 0.0f || time1 > 1.0f || time2 < 0.0f || time2 > 1.0f)
        return 0;

    CgeVec2fLerp(aStart, aEnd, time1, out);
    *t = time1;

    return 1;
}

int CgeRay2fIntersectBox2f(const float aStart[2], const float aDirection[2],
                           const float bMin[2], const float bMax[2],
                           float *t, float out[2]) {
    float timeNear, timeFar, hitNear, hitFar, denom, tmp;
    int i;

    timeNear = -1.0f / 0.0f;
    timeFar  =  1.0f / 0.0f;

    /* Check if origin inside box */
    if (!CgeBox2fContains(bMin, bMax, aStart)) {
        memcpy(out, aStart, sizeof(float) * 2);
        *t = 0.0f;
        return 1;
    }

    /* Check each axis for the minimal and maximum intersection time */
    for (i = 0; i < 2; i++) {
        if (fabsf(aDirection[i]) < EPSILON) {
            if (aStart[i] < bMin[i] || aStart[i] > bMax[i])
                return 0;
            continue;
        }

        denom = 1.0f / aDirection[i];
        hitNear = (bMin[i] - aStart[i]) * denom;
        hitFar  = (bMax[i] - aStart[i]) * denom;

        if (hitNear > hitFar) {
            tmp = hitNear;
            hitNear = hitFar;
            hitFar = tmp;
        }

        if (hitNear > timeNear)
            timeNear = hitNear;
        if (hitFar < timeFar)
            timeFar = hitFar;

        if (timeNear > timeFar || timeFar < 0.0f)
            return 0;
    }

    out[0] = aStart[0] + aDirection[0] * timeNear;
    out[1] = aStart[1] + aDirection[1] * timeNear;
    *t = timeNear;

    return 1;
}

int CgeSegment2fIntersectBox2f(const float aStart[2], const float aEnd[2],
                               const float bMin[2], const float bMax[2],
                               float *t, float out[2]) {
    float tmp[3];
    float time;

    CgeVec2fSub(aEnd, aStart, tmp);
    if (CgeRay2fIntersectBox2f(aStart, tmp, bMin, bMax, &time, out))
        return 0;

    if (time > 1.0f)
        return 0;

    *t = time;
    return 1;
}
