#include "CgeMath.h"
#include <math.h>

#define EPSILON 0.00001f
#define PI      3.14159265358979323846f

int CgeRay2fIntersectLine(const float start[2], const float direction[2],
                          const float line[3], float *t, float out[2]) {
    float tmp1[2];
    float denom, time;

    denom = CgeVec2fDot(direction, line);
    time = (line[2] - CgeVec2fDot(line, start)) / denom;

    if (fabsf(denom) < EPSILON || time < 0.0f)
        return 0;

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

    tmp1[0] = -aDirection[1]; tmp1[1] = aDirection[0];
    tmp2[0] = -bDirection[1]; tmp2[1] = bDirection[0];
    denom = CgeVec2fDot(tmp1, bDirection);

    if (fabsf(denom) < EPSILON)
        return 0;

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

    if (!CgeRay2fIntersectTime(aStart, aDirection, bStart, bDirection, &time1, &time2))
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
    if (!CgeRay2fIntersectTime(aStart, aDirection, bStart, tmp, &time1, &time2))
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

    CgeVec2fSub(end, start, tmp);
    denom = CgeVec2fDot(tmp, line);
    time = (line[2] - CgeVec2fDot(line, start)) / denom;

    if (fabsf(denom) < EPSILON || time < 0.0f || time > 1.0f)
        return 0;

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
    if (!CgeRay2fIntersectTime(aStart, tmp1, bStart, tmp2, &time1, &time2))
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
    if (!CgeRay2fIntersectBox2f(aStart, tmp, bMin, bMax, &time, out))
        return 0;

    if (time > 1.0f)
        return 0;

    *t = time;
    return 1;
}

int CgeRay2fIntersectCircle(const float start[2], const float direction[2],
                            const float center[2], float radius, float *t,
                            float out[2])
{
    float v[2], tmp[2];
    float tCa, d2, th, t1, t2;

    CgeVec2fSub(center, start, v);
    tCa = CgeVec2fDot(v, direction);
    d2 = CgeVec2fDot(v, v) - tCa * tCa;

    if (d2 > radius * radius)
        return 0;

    th = (float)sqrt(radius * radius - d2);
    t1 = tCa - th;
    t2 = tCa + th;

    if (t2 < EPSILON) return 0;

    *t = (t1 > EPSILON) ? t1 : t2;
    CgeVec2fScale(direction, *t, tmp);
    CgeVec2fAdd(start, tmp, out);

    return 1;
}

int CgeSegment2fIntersectCircle(const float start[2], const float end[2],
                                const float center[2], float radius, float *t,
                                float out[2]) {
    float dir[2], lenSq, len;
    float norm[2];

    CgeVec2fSub(end, start, dir);
    lenSq = CgeVec2fDot(dir, dir);

    if (lenSq == 0.0f)
        return 0;

    len = (float)sqrt(lenSq);
    CgeVec2fScale(dir, 1.0f / len, norm);

    if (!CgeRay2fIntersectCircle(start, norm, center, radius, t, out)) {
        return 0;
    }

    *t = *t / len;
    if (*t > len)
        return 0;

    CgeVec2fScale(dir, *t, out);
    CgeVec2fAdd(start, out, out);
    return 1;
}
