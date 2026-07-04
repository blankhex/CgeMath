#include <stdio.h>
#include <math.h>
#include "CgeMath.h"

#define WIDTH        640
#define HEIGHT       480
#define BG_R         0.05f
#define BG_G         0.05f
#define BG_B         0.1f
#define MAX_BOUNCES  7

static int toInt(float x) {
    return (int)(x * 255.0f);
}

typedef struct {
    float center[3];
    float radius;
    const float *color;
    float reflectivity;
    float shininess;
    float specular;
} Sphere;

static const float red[3] = {1.0f, 0.0f, 0.0f};
static const float green[3] = {0.0f, 1.0f, 0.0f};
static const float blue[3] = {0.0f, 0.0f, 1.0f};

static const Sphere spheres[] = {
    {{-2.0f, 0.0f, -5.0f}, 1.0f, red, 0.1f, 20.0f, 0.5f},
    {{ 0.0f, 2.0f, -4.0f}, 1.0f, green, 0.5f, 50.0f, 0.8f},
    {{ 2.0f, 0.0f, -5.0f}, 1.0f, blue, 0.95f, 100.0f, 1.0f}
};

static const int numSpheres = 3;
static const float lightPos[3] = {0.0f, 3.0f, -2.0f};
static const float ambient = 0.2f;
static const float origin[3] = {0.0f, 0.5f, 0.0f};

int traceRay(const float origin[3], const float direction[3], float *t,
             int *hitIndex, float hitPoint[3], float normal[3]) {
    float t0, closest;
    float p[3];
    int i, hit;

    closest = 1e10f;
    hit = -1;

    for (i = 0; i < numSpheres; i++) {
        if (CgeRay3fIntersectSphere(origin, direction, spheres[i].center, spheres[i].radius, &t0, p) == 1) {
            if (t0 > 0.001f && t0 < closest) {
                closest = t0;
                hit = i;
                CgeVec3fSub(p, spheres[i].center, normal);
                CgeVec3fNormal(normal, normal);
            }
        }
    }

    if (direction[1] != 0.0f) {
        float tPlane;
        tPlane = (-1.0f - origin[1]) / direction[1];
        if (tPlane > 0.001f && tPlane < closest) {
            closest = tPlane;
            hit = -2;
        }
    }

    if (closest == 1e10f) {
        return -1;
    }

    *t = closest;
    *hitIndex = hit;

    hitPoint[0] = origin[0] + closest * direction[0];
    hitPoint[1] = origin[1] + closest * direction[1];
    hitPoint[2] = origin[2] + closest * direction[2];

    if (hit >= 0) {
        CgeVec3fSub(hitPoint, spheres[hit].center, normal);
        CgeVec3fNormal(normal, normal);
    } else if (hit == -2) {
        normal[0] = 0.0f; normal[1] = 1.0f; normal[2] = 0.0f;
    }

    return 1;
}

void traceColor(const float origin[3], const float direction[3], float color[3],
                int depth) {
    float viewDir[3], halfVec[3], reflectDir[3], offset[3], reflectOrigin[3];
    float hitPoint[3], normal[3], localColor[3], reflectColor[3];
    float toLight[3], lightDir[3], dummyPoint[3], dummyNormal[3];
    float t, NdotL, diffuse, specular, brightness, tile, NdotH, reflectivity;
    float shadow, tShadow;
    const float *surf;
    int hitIndex, dummyIdx, ix, iz;

    if (depth >= MAX_BOUNCES) {
        color[0] = color[1] = color[2] = 0.0f;
        return;
    }

    if (traceRay(origin, direction, &t, &hitIndex, hitPoint, normal) >= 0) {
        CgeVec3fSub(lightPos, hitPoint, toLight);
        CgeVec3fNormal(toLight, lightDir);

        shadow = 1.0f;
        if (traceRay(hitPoint, lightDir, &tShadow, &dummyIdx, dummyPoint, dummyNormal) >= 0) {
            float lightDist = CgeVec3fDistance(lightPos, hitPoint);
            if (tShadow < lightDist) {
                shadow = 0.0f;
            }
        }

        NdotL = CgeVec3fDot(normal, lightDir);
        diffuse = (NdotL > 0.0f) ? NdotL : 0.0f;

        specular = 0.0f;
        if (hitIndex >= 0 && NdotL > 0.0f) {
            CgeVec3fSub(origin, hitPoint, viewDir);
            CgeVec3fNormal(viewDir, viewDir);

            CgeVec3fAdd(viewDir, lightDir, halfVec);
            CgeVec3fNormal(halfVec, halfVec);

            NdotH = CgeVec3fDot(normal, halfVec);
            if (NdotH > 0.0f) {
                specular = powf(NdotH, spheres[hitIndex].shininess) * spheres[hitIndex].specular;
            }
        }

        brightness = ambient + diffuse * shadow + specular * shadow;

        if (hitIndex == -2) {
            ix = (int)floorf(hitPoint[0]);
            iz = (int)floorf(hitPoint[2]);
            tile = ((ix + iz) % 2 == 0) ? 1.0f : 0.3f;
            localColor[0] = localColor[1] = localColor[2] = tile * brightness;
        } else {
            surf = spheres[hitIndex].color;
            localColor[0] = surf[0] * brightness;
            localColor[1] = surf[1] * brightness;
            localColor[2] = surf[2] * brightness;
        }

        reflectivity = (hitIndex >= 0) ? spheres[hitIndex].reflectivity : 0.0f;
        if (reflectivity > 0.0f) {
            CgeVec3fReflect(direction, normal, reflectDir);
            CgeVec3fScale(reflectDir, 0.001f, offset);
            CgeVec3fAdd(hitPoint, offset, reflectOrigin);

            traceColor(reflectOrigin, reflectDir, reflectColor, depth + 1);

            localColor[0] = localColor[0] * (1.0f - reflectivity) + reflectColor[0] * reflectivity;
            localColor[1] = localColor[1] * (1.0f - reflectivity) + reflectColor[1] * reflectivity;
            localColor[2] = localColor[2] * (1.0f - reflectivity) + reflectColor[2] * reflectivity;
        }

        color[0] = localColor[0];
        color[1] = localColor[1];
        color[2] = localColor[2];
    } else {
        color[0] = BG_R;
        color[1] = BG_G;
        color[2] = BG_B;
    }
}

int main() {
    int x, y;
    float u, v, aspect, fov;
    float direction[3], color[3];

    printf("P3\n%d %d\n255\n", WIDTH, HEIGHT);

    aspect = (float)WIDTH / (float)HEIGHT;
    fov = 1.0f;

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            u = (2.0f * (x + 0.5f) / (float)WIDTH - 1.0f) * aspect;
            v = (2.0f * (HEIGHT - y - 0.5f) / (float)HEIGHT - 1.0f);

            direction[0] = u;
            direction[1] = v;
            direction[2] = -fov;
            CgeVec3fNormal(direction, direction);
            traceColor(origin, direction, color, 0);

            if (color[0] < 0.0f) color[0] = 0.0f; else if (color[0] > 1.0f) color[0] = 1.0f;
            if (color[1] < 0.0f) color[1] = 0.0f; else if (color[1] > 1.0f) color[1] = 1.0f;
            if (color[2] < 0.0f) color[2] = 0.0f; else if (color[2] > 1.0f) color[2] = 1.0f;

            color[0] = powf(color[0], 1.0f / 2.2f);
            color[1] = powf(color[1], 1.0f / 2.2f);
            color[2] = powf(color[2], 1.0f / 2.2f);

            printf("%d %d %d\n", toInt(color[0]), toInt(color[1]), toInt(color[2]));
        }
    }

    return 0;
}
