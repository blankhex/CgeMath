#ifndef CGE_MATH_H
#define CGE_MATH_H

#include <stddef.h>

void CgeBox2fUnion(const float aMin[2], const float aMax[2],
                   const float bMin[2], const float bMax[2],
                   float outMin[2], float outMax[2]);
int CgeBox2fIntersect(const float aMin[2], const float aMax[2],
                      const float bMin[2], const float bMax[2],
                      float outMin[2], float outMax[2]);
int CgeBox2fContains(const float aMin[2], const float aMax[2],
                     const float point[2]);
int CgeBox2fEnclose(const float *points, size_t size, float outMin[2],
                    float outMax[2]);

void CgeBox3fUnion(const float aMin[3], const float aMax[3],
                   const float bMin[3], const float bMax[3],
                   float outMin[3], float outMax[3]);
int CgeBox3fIntersect(const float aMin[3], const float aMax[3],
                      const float bMin[3], const float bMax[3],
                      float outMin[3], float outMax[3]);
int CgeBox3fContains(const float aMin[3], const float aMax[3],
                     const float point[3]);
int CgeBox3fEnclose(const float *points, size_t size, float outMin[3],
                    float outMax[3]);

int CgeLineFromPoints(const float a[2], const float b[2], float out[3]);
float CgeLineDistance(const float line[3], const float point[2]);
void CgeLineClosestPoint(const float line[3], const float point[2],
                         float out[2]);

void CgeMat3fIdentity(float out[9]);
void CgeMat3fAdd(const float a[9], const float b[9], float out[9]);
void CgeMat3fSub(const float a[9], const float b[9], float out[9]);
void CgeMat3fMul(const float a[9], const float b[9], float out[9]);
void CgeMat3fScale(const float a[9], float b, float out[9]);
void CgeMat3fTranspose(const float in[9], float out[9]);
float CgeMat3fTrace(const float in[9]);
float CgeMat3fDet(const float in[9]);
int CgeMat3fInverse(const float in[9], float out[9]);
void CgeMat3fFromScale(float x, float y, float out[9]);
void CgeMat3fFromTranslation(float x, float y, float out[9]);
void CgeMat3fFromRotation(float angle, float out[9]);
void CgeMat3fApplyVec3f(float a[9], float b[3], float out[3]);
void CgeMat3fApplyVec2f(float a[9], float b[2], float out[2]);

void CgeMat4fIdentity(float out[16]);
void CgeMat4fAdd(const float a[16], const float b[16], float out[16]);
void CgeMat4fSub(const float a[16], const float b[16], float out[16]);
void CgeMat4fMul(const float a[16], const float b[16], float out[16]);
void CgeMat4fScale(const float a[16], float b, float out[16]);
void CgeMat4fTranspose(const float in[16], float out[16]);
float CgeMat4fTrace(const float in[16]);
float CgeMat4fDet(const float in[16]);
int CgeMat4fInverse(const float in[16], float out[16]);
void CgeMat4fFromScale(float x, float y, float z, float out[16]);
void CgeMat4fFromTranslation(float x, float y, float z, float out[16]);
void CgeMat4fFromRotationX(float angle, float out[16]);
void CgeMat4fFromRotationY(float angle, float out[16]);
void CgeMat4fFromRotationZ(float angle, float out[16]);
void CgeMat4fFromAxis(const float axis[3], float angle, float out[16]);
void CgeMat4fFromEuler(float roll, float pitch, float yaw, float out[16]);
void CgeMat4fFromQuat4f(const float in[4], float out[16]);
void CgeMat4fFromOrtho(float xMin, float xMax, float yMin, float yMax,
                       float zMin, float zMax, float out[16]);
void CgeMat4fFromFrustum(float fov, float aspect, float zMin, float zMax,
                         float out[16]);
void CgeMat4fFromLookAt(const float position[3], const float at[3],
                        const float up[3], float out[16]);
void CgeMat4fApplyVec4f(const float a[16], const float b[4], float out[4]);
void CgeMat4fApplyVec3f(const float a[16], const float b[3], float out[3]);

float CgeLerpf(float a, float b, float t);

void CgeTriangle3fBarycentric(const float a[3], const float b[3],
                              const float c[3], const float point[3],
                              float out[3]);

int CgePlaneFromPoints(const float a[3], const float b[3], const float c[3],
                       float out[4]);
float CgePlaneDistance(const float plane[4], const float point[3]);

void CgePlaneClosestPoint(const float plane[4], const float point[3],
                          float out[3]);

#define CgeQuat4fAdd(a, b, out) CgeVec4fAdd(a, b, out)
#define CgeQuat4fSub(a, b, out) CgeVec4fSub(a, b, out)
#define CgeQuat4fScale(a, b, out) CgeVec4fScale(a, b, out)
#define CgeQuat4fNegate(in, out) CgeVec4fNegate(in, out)
#define CgeQuat4fDot(a, b) CgeVec4fDot(a, b)
#define CgeQuat4fLength(in) CgeVec4fLength(in)
#define CgeQuat4fNormal(in, out) CgeVec4fNormal(in, out)
#define CgeQuat4fLerp(a, b, t, out) CgeVec4fLerp(a, b, t, out)

void CgeQuat4fIdentity(float out[4]);
void CgeQuat4fConjugate(const float in[4],float out[4]);
void CgeQuat4fInverse(const float in[4], float out[4]);
void CgeQuat4fMul(const float a[4], const float b[4], float out[4]);
void CgeQuat4fSlerp(const float a[4], const float b[4], float t, float out[4]);
void CgeQuat4fFromEuler(float roll, float pitch, float yaw, float out[4]);
void CgeQuat4fFromAxis(const float axis[3], float angle, float out[4]);
void CgeQuat4fToEuler(const float in[4], float *roll, float *pitch, float *yaw);
void CgeQuat4fToAxis(const float in[4], float axis[3], float *angle);
void CgeQuat4fToMat4f(const float in[4], float out[16]);

int CgeRay2fIntersectLine(const float start[2], const float direction[2],
                          const float line[3], float *t, float out[2]);
int CgeRay2fIntersectTime(const float aStart[2], const float aDirection[2],
                          const float bStart[2], const float bDirection[2],
                          float *time1, float *time2);
int CgeRay2fIntersectRay(const float aStart[2], const float aDirection[2],
                         const float bStart[2], const float bDirection[2],
                         float *t, float out[2]);
int CgeRay2fIntersectSegment(const float aStart[2], const float aDirection[2],
                             const float bStart[2], const float bEnd[2],
                             float *t, float out[2]);
int CgeSegment2fIntersectLine(const float start[2], const float end[2],
                              const float line[3], float *t, float out[2]);
int CgeSegment2fIntersectSegment(const float aStart[2], const float aEnd[2],
                                 const float bStart[2], const float bEnd[2],
                                 float *t, float out[2]);
int CgeRay2fIntersectBox2f(const float aStart[2], const float aDirection[2],
                           const float bMin[2], const float bMax[2],
                           float *t, float out[2]);
int CgeSegment2fIntersectBox2f(const float aStart[2], const float aEnd[2],
                               const float bMin[2], const float bMax[2],
                               float *t, float out[2]);
int CgeRay2fIntersectCircle(const float start[2], const float direction[2],
                            const float center[2], float radius, float *t,
                            float out[2]);
int CgeSegment2fIntersectCircle(const float start[2], const float end[2],
                                const float center[2], float radius, float *t,
                                float out[2]);

int CgeRay3fIntersectPlane(const float start[3], const float direction[3],
                           const float plane[4], float *t, float out[3]);
int CgeRay3fIntersectTriangle(const float start[3], const float direction[3],
                              const float a[3], const float b[3],
                              const float c[3], float *t, float out[3]);
int CgeSegment3fIntersectPlane(const float start[3], const float end[3],
                               const float plane[4], float *t, float out[3]);
int CgeSegment3fIntersectTriangle(const float start[3], const float end[3],
                                  const float a[3], const float b[3],
                                  const float c[3], float *t, float out[3]);
int CgeRay3fIntersectBox3f(const float aStart[3], const float aDirection[3],
                           const float bMin[3], const float bMax[3],
                           float *t, float out[3]);
int CgeSegment3fIntersectBox3f(const float aStart[3], const float aEnd[3],
                               const float bMin[3], const float bMax[3],
                               float *t, float out[3]);
int CgeRay3fIntersectSphere(const float start[3], const float direction[3],
                            const float center[3], float radius, float *t,
                            float out[3]);
int CgeSegment3fIntersectSphere(const float start[3], const float end[3],
                                const float center[3], float radius, float *t,
                                float out[3]);

void CgeVec2fAdd(const float a[2], const float b[2], float out[2]);
void CgeVec2fSub(const float a[2], const float b[2], float out[2]);
void CgeVec2fMul(const float a[2], const float b[2], float out[2]);
void CgeVec2fScale(const float a[2], float b, float out[2]);
void CgeVec2fMulAdd(const float a[2], const float b[2], const float c[2],
                    float out[2]);
void CgeVec2fNegate(const float in[2], float out[2]);
float CgeVec2fDot(const float a[2], const float b[2]);
float CgeVec2fCross(const float a[2], const float b[2]);
float CgeVec2fLength(const float in[2]);
void CgeVec2fNormal(const float in[2], float out[2]);
float CgeVec2fNormalEx(const float in[2], float out[2]);
void CgeVec2fMin(const float a[2], const float b[2], float out[2]);
void CgeVec2fMax(const float a[2], const float b[2], float out[2]);
void CgeVec2fLerp(const float a[2], const float b[2], float t, float out[2]);
void CgeVec2fProject(const float a[2], const float b[2], float out[2]);
void CgeVec2fBarycentric(const float a[2], const float b[2], const float c[2],
                         float v, float w, float out[2]);
float CgeVec2fDistance(const float a[2], const float b[2]);
void CgeVec2fReflect(const float in[2], const float norm[2], float out[2]);
void CgeVec2fRefract(const float in[2], const float norm[2], float eta,
                     float out[2]);
void CgeVec2fClamp(const float in[2], float minVal, float maxVal, float out[2]);
void CgeVec2fAbs(const float in[2], float out[2]);

void CgeVec2iAdd(const int a[2], const int b[2], int out[2]);
void CgeVec2iSub(const int a[2], const int b[2], int out[2]);
void CgeVec2iMul(const int a[2], const int b[2], int out[2]);
void CgeVec2iScale(const int a[2], int b, int out[2]);
void CgeVec2iMulAdd(const int a[2], const int b[2], const int c[2], int out[2]);
void CgeVec2iNegate(const int in[2], int out[2]);
void CgeVec2iMin(const int a[2], const int b[2], int out[2]);
void CgeVec2iMax(const int a[2], const int b[2], int out[2]);
void CgeVec2iClamp(const int in[2], int minVal, int maxVal, int out[2]);
void CgeVec2iAbs(const int in[2], int out[2]);

void CgeVec3fAdd(const float a[3], const float b[3], float out[3]);
void CgeVec3fSub(const float a[3], const float b[3], float out[3]);
void CgeVec3fMul(const float a[3], const float b[3], float out[3]);
void CgeVec3fScale(const float a[3], float b, float out[3]);
void CgeVec3fMulAdd(const float a[3], const float b[3], const float c[3],
                    float out[3]);
void CgeVec3fNegate(const float in[3], float out[3]);
float CgeVec3fDot(const float a[3], const float b[3]);
void CgeVec3fCross(const float a[3], const float b[3], float out[3]);
float CgeVec3fLength(const float in[3]);
void CgeVec3fNormal(const float in[3], float out[3]);
float CgeVec3fNormalEx(const float in[3], float out[3]);
void CgeVec3fMin(const float a[3], const float b[3], float out[3]);
void CgeVec3fMax(const float a[3], const float b[3], float out[3]);
void CgeVec3fLerp(const float a[3], const float b[3], float t, float out[3]);
void CgeVec3fProject(const float a[3], const float b[3], float out[3]);
void CgeVec3fBarycentric(const float a[3], const float b[3], const float c[3],
                         float v, float w, float out[3]);
float CgeVec3fDistance(const float a[3], const float b[3]);
void CgeVec3fReflect(const float in[3], const float norm[3], float out[3]);
void CgeVec3fRefract(const float in[3], const float norm[3], float eta,
                     float out[3]);
void CgeVec3fClamp(const float in[3], float minVal, float maxVal, float out[3]);
void CgeVec3fAbs(const float in[3], float out[3]);

void CgeVec3iAdd(const int a[3], const int b[3], int out[3]);
void CgeVec3iSub(const int a[3], const int b[3], int out[3]);
void CgeVec3iMul(const int a[3], const int b[3], int out[3]);
void CgeVec3iScale(const int a[3], int b, int out[3]);
void CgeVec3iMulAdd(const int a[3], const int b[3], const int c[3], int out[3]);
void CgeVec3iNegate(const int in[3], int out[3]);
void CgeVec3iMin(const int a[3], const int b[3], int out[3]);
void CgeVec3iMax(const int a[3], const int b[3], int out[3]);
void CgeVec3iClamp(const int in[3], int minVal, int maxVal, int out[3]);
void CgeVec3iAbs(const int in[3], int out[3]);

void CgeVec4fAdd(const float a[4], const float b[4], float out[4]);
void CgeVec4fSub(const float a[4], const float b[4], float out[4]);
void CgeVec4fMul(const float a[4], const float b[4], float out[4]);
void CgeVec4fScale(const float a[4], float b, float out[4]);
void CgeVec4fMulAdd(const float a[4], const float b[4], const float c[4],
                    float out[4]);
void CgeVec4fNegate(const float in[4], float out[4]);
float CgeVec4fDot(const float a[4], const float b[4]);
float CgeVec4fLength(const float in[4]);
void CgeVec4fNormal(const float in[4], float out[4]);
float CgeVec4fNormalEx(const float in[4], float out[4]);
void CgeVec4fMin(const float a[4], const float b[4], float out[4]);
void CgeVec4fMax(const float a[4], const float b[4], float out[4]);
void CgeVec4fLerp(const float a[4], const float b[4], float t, float out[4]);
void CgeVec4fProject(const float a[4], const float b[4], float out[4]);
void CgeVec4fBarycentric(const float a[4], const float b[4], const float c[4],
                         float v, float w, float out[4]);
float CgeVec4fDistance(const float a[4], const float b[4]);
void CgeVec4fClamp(const float in[4], float minVal, float maxVal, float out[4]);
void CgeVec4fAbs(const float in[4], float out[4]);

void CgeVec4iAdd(const int a[4], const int b[4], int out[4]);
void CgeVec4iSub(const int a[4], const int b[4], int out[4]);
void CgeVec4iMul(const int a[4], const int b[4], int out[4]);
void CgeVec4iScale(const int a[4], int b, int out[4]);
void CgeVec4iMulAdd(const int a[4], const int b[4], const int c[4], int out[4]);
void CgeVec4iNegate(const int in[4], int out[4]);
void CgeVec4iMin(const int a[4], const int b[4], int out[4]);
void CgeVec4iMax(const int a[4], const int b[4], int out[4]);
void CgeVec4iClamp(const int in[4], int minVal, int maxVal, int out[4]);
void CgeVec4iAbs(const int in[4], int out[4]);

#endif /* CGE_MATH_H */
