#include "CgeMath.h"
#include <string.h>
#include <math.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

int CgeRay3fIntersectPlane(const float start[3], const float direction[3],
                           const float plane[4], float *t, float out[3]) {
    float tmp1[3];
    float denom, time;

    denom = CgeVec3fDot(direction, plane);
    time = (plane[3] - CgeVec3fDot(plane, start)) / denom;

    if (fabsf(denom) < EPSILON || time < 0.0f)
        return 0;

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

    if (!CgePlaneFromPoints(a, b, c, plane))
        return 0;

    if (!CgeRay3fIntersectPlane(start, direction, plane, &time, tmp3))
        return 0;

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

    memcpy(out, tmp3, sizeof(tmp3));
    *t = time;
    return 1;
}

int CgeSegment3fIntersectPlane(const float start[3], const float end[3],
                               const float plane[4], float *t, float out[3]) {
    float tmp[3];
    float denom, time;

    CgeVec3fSub(end, start, tmp);
    denom = CgeVec3fDot(tmp, plane);
    time = (plane[3] - CgeVec3fDot(plane, start)) / denom;

    if (fabsf(denom) < EPSILON || time < 0.0f || time > 1.0f)
        return 0;

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

    if (!CgePlaneFromPoints(a, b, c, plane))
        return 0;

    if (!CgeSegment3fIntersectPlane(start, end, plane, &time, tmp3))
        return 0;

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
    if (!CgeRay3fIntersectBox3f(aStart, tmp, bMin, bMax, &time, out))
        return 0;

    if (time > 1.0f)
        return 0;

    *t = time;
    return 1;
}

int CgeRay3fIntersectSphere(const float start[3], const float direction[3],
                            const float center[3], float radius, float *t,
                            float out[3])
{
    float v[3], tmp[3];
    float tCa, d2, th, t1, t2;

    CgeVec3fSub(center, start, v);
    tCa = CgeVec3fDot(v, direction);
    d2 = CgeVec3fDot(v, v) - tCa * tCa;

    if (d2 > radius * radius)
        return 0;

    th = (float)sqrt(radius * radius - d2);
    t1 = tCa - th;
    t2 = tCa + th;

    if (t2 < EPSILON) return 0;

    *t = (t1 > EPSILON) ? t1 : t2;
    CgeVec3fScale(direction, *t, tmp);
    CgeVec3fAdd(start, tmp, out);

    return 1;
}

int CgeSegment3fIntersectSphere(const float start[3], const float end[3],
                                const float center[3], float radius, float *t,
                                float out[3]) {
    float dir[3], lenSq, len;
    float norm[3];

    CgeVec3fSub(end, start, dir);
    lenSq = CgeVec3fDot(dir, dir);

    if (lenSq == 0.0f)
        return 0;

    len = (float)sqrt(lenSq);
    CgeVec3fScale(dir, 1.0f / len, norm);

    if (!CgeRay3fIntersectSphere(start, norm, center, radius, t, out)) {
        return 0;
    }

    *t = *t / len;
    if (*t > len)
        return 0;

    CgeVec3fScale(dir, *t, out);
    CgeVec3fAdd(start, out, out);
    return 1;
}
