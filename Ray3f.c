#include "CgeMath.h"
#include <string.h>
#include <math.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

int CgeRay3fIntersectPlane(const float start[3], const float direction[3],
                           const float plane[4], float *t, float out[3]) {
    float tmp1[3];
    float denom, time;

    /* Calculate intersection time */
    denom = CgeVec3fDot(direction, plane);
    time = (plane[3] - CgeVec3fDot(plane, start)) / denom;

    /* Check for ray/plane parallel to each other or point is behing the ray. */
    if (fabsf(denom) < EPSILON || time < 0.0f)
        return 0;

    /* Compute intersection point */
    CgeVec3fScale(direction, time, tmp1);
    CgeVec3fAdd(start, tmp1, out);
    *t = time;
    return 1;
}

int CgeRay3fIntersectTriangle(const float start[3], const float direction[3],
                              const float a[3], const float b[3],
                              const float c[3], float *t, float out[3]) {
    float plane[4];
    float tmp1[3], tmp2[3], tmp3[3];
    float time;

    /* Compute plane */
    if (CgePlaneFromPoints(a, b, c, plane))
        return 0;

    /* Compute intersection point in ray against plane */
    if (CgeRay3fIntersectPlane(start, direction, plane, &time, tmp3))
        return 0;

    /* Check if point inside rectangle */
    CgeVec3fSub(b, a, tmp1);
    CgeVec3fSub(tmp3, a, tmp2);
    CgeVec3fCross(tmp2, tmp1, tmp1);
    if (CgeVec3fDot(tmp1, plane) < 0.0f)
        return 0;

    CgeVec3fSub(c, b, tmp1);
    CgeVec3fSub(tmp3, b, tmp2);
    CgeVec3fCross(tmp2, tmp1, tmp1);
    if (CgeVec3fDot(tmp1, plane) < 0.0f)
        return 0;

    CgeVec3fSub(a, c, tmp1);
    CgeVec3fSub(tmp3, c, tmp2);
    CgeVec3fCross(tmp2, tmp1, tmp1);
    if (CgeVec3fDot(tmp1, plane) < 0.0f)
        return 0;

    /* Copy intersection point */
    memcpy(out, tmp3, sizeof(tmp3));
    *t = time;
    return 1;
}

int CgeSegment3fIntersectPlane(const float start[3], const float end[3],
                               const float plane[4], float *t, float out[3]) {
    float tmp[3];
    float denom, time;

    /* Calculate intersection time */
    CgeVec3fSub(end, start, tmp);
    denom = CgeVec3fDot(tmp, plane);
    time = (plane[3] - CgeVec3fDot(plane, start)) / denom;

    /* Check for ray/plane parallel to each other or point is behing the ray. */
    if (fabsf(denom) < EPSILON || time < 0.0f || time > 1.0f)
        return 0;

    /* Compute intersection point */
    CgeVec3fScale(tmp, time, tmp);
    CgeVec3fAdd(start, tmp, out);
    *t = time;
    return 1;
}

int CgeSegment3fIntersectTriangle(const float start[3], const float end[3],
                                  const float a[3], const float b[3],
                                  const float c[3], float *t, float out[3]) {
    float plane[4];
    float tmp1[3], tmp2[3], tmp3[3];
    float time;

    /* Compute plane */
    if (CgePlaneFromPoints(a, b, c, plane))
        return 0;

    /* Compute intersection point in ray against plane */
    if (CgeSegment3fIntersectPlane(start, end, plane, &time, tmp3))
        return 0;

    /* Check if point inside rectangle */
    CgeVec3fSub(b, a, tmp1);
    CgeVec3fSub(tmp3, a, tmp2);
    CgeVec3fCross(tmp2, tmp1, tmp1);
    if (CgeVec3fDot(tmp1, plane) < 0.0f)
        return 0;

    CgeVec3fSub(c, b, tmp1);
    CgeVec3fSub(tmp3, b, tmp2);
    CgeVec3fCross(tmp2, tmp1, tmp1);
    if (CgeVec3fDot(tmp1, plane) < 0.0f)
        return 0;

    CgeVec3fSub(a, c, tmp1);
    CgeVec3fSub(tmp3, c, tmp2);
    CgeVec3fCross(tmp2, tmp1, tmp1);
    if (CgeVec3fDot(tmp1, plane) < 0.0f)
        return 0;

    /* Copy intersection point */
    memcpy(out, tmp3, sizeof(tmp3));
    *t = time;
    return 1;
}

int CgeRay3fIntersectBox3f(const float aStart[3], const float aDirection[3],
                           const float bMin[3], const float bMax[3], float *t,
                           float out[3]) {
    float timeNear, timeFar, hitNear, hitFar, denom, tmp;
    int i;

    timeNear = -1.0f / 0.0f;
    timeFar  =  1.0f / 0.0f;

    /* Check if origin inside box */
    if (!CgeBox3fContains(bMin, bMax, aStart)) {
        memcpy(out, aStart, sizeof(float) * 3);
        *t = 0.0f;
        return 1;
    }

    /* Check each axis for the minimal and maximum intersection time */
    for (i = 0; i < 3; i++) {
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
    out[2] = aStart[2] + aDirection[2] * timeNear;
    *t = timeNear;

    return 1;
}

int CgeSegment3fIntersectBox3f(const float aStart[3], const float aEnd[3],
                               const float bMin[3], const float bMax[3],
                               float *t, float out[3]) {
    float tmp[3];
    float time;

    CgeVec3fSub(aEnd, aStart, tmp);
    if (CgeRay3fIntersectBox3f(aStart, tmp, bMin, bMax, &time, out))
        return 0;

    if (time > 1.0f)
        return 0;

    *t = time;
    return 1;
}
