#include <math.h>
#include <stdio.h>
#include <string.h>
#include "CgeMath.h"

#define WIDTH        640
#define HEIGHT       480
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

static const float bg[3] = {0.05f, 0.05f, 0.1f};
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
    int i;

    closest = 1.0 / 0.0;
    *hitIndex = -1;
    for (i = 0; i < numSpheres; i++) {
        if (CgeRay3fIntersectSphere(origin, direction, spheres[i].center, spheres[i].radius, &t0, p)) {
            if (t0 > 0.001f && t0 < closest) {
                closest = t0;
                *t = t0;
                *hitIndex = i;

                memcpy(hitPoint, p, sizeof(p));
                CgeVec3fSub(hitPoint, spheres[i].center, normal);
                CgeVec3fNormal(normal, normal);
            }
        }
    }

    t0 = (-1.0f - origin[1]) / direction[1];
    if (t0 > 0.001f && t0 < closest) {
        closest = t0;
        *t = t0;
        *hitIndex = -2;

        CgeVec3fScale(direction, closest, hitPoint);
        CgeVec3fAdd(origin, hitPoint, hitPoint);
        normal[0] = 0.0f; normal[1] = 1.0f; normal[2] = 0.0f;
    }

    return *hitIndex != -1;
}

void traceColor(const float origin[3], const float direction[3], float color[3],
                int depth) {
    float normal[3], hitPoint[3];
    float t;
    int hitIndex;

    if (depth >= MAX_BOUNCES) {
        color[0] = color[1] = color[2] = 0.0f;
        return;
    }

    if (traceRay(origin, direction, &t, &hitIndex, hitPoint, normal)) {
        float localColor[3], toLight[3], lightDir[3];
        float dummyPoint[3], dummyNormal[3];
        float NdotL, diffuse, specular, brightness, reflectivity;
        float shadow;
        int dummyIdx;

        CgeVec3fSub(lightPos, hitPoint, toLight);
        CgeVec3fNormal(toLight, lightDir);

        shadow = 1.0f;
        if (traceRay(hitPoint, lightDir, &t, &dummyIdx, dummyPoint, dummyNormal)) {
            float lightDist = CgeVec3fDistance(lightPos, hitPoint);
            if (t < lightDist) {
                shadow = 0.0f;
            }
        }

        NdotL = CgeVec3fDot(normal, lightDir);
        diffuse = (NdotL > 0.0f) ? NdotL : 0.0f;

        specular = 0.0f;
        if (hitIndex >= 0 && NdotL > 0.0f) {
            float viewDir[3], halfVec[3];
            float NdotH;

            CgeVec3fSub(origin, hitPoint, viewDir);
            CgeVec3fNormal(viewDir, viewDir);

            CgeVec3fAdd(viewDir, lightDir, halfVec);
            CgeVec3fNormal(halfVec, halfVec);

            NdotH = CgeVec3fDot(normal, halfVec);
            if (NdotH > 0.0f)
                specular = powf(NdotH, spheres[hitIndex].shininess) * spheres[hitIndex].specular;
        }

        brightness = ambient + diffuse * shadow + specular * shadow;

        if (hitIndex == -2) {
            float tile;
            int ix, iz;

            ix = (int)floorf(hitPoint[0]);
            iz = (int)floorf(hitPoint[2]);
            tile = ((ix + iz) % 2 == 0) ? 1.0f : 0.3f;
            localColor[0] = localColor[1] = localColor[2] = tile * brightness;
        } else {
            CgeVec3fScale(spheres[hitIndex].color, brightness, localColor);
        }

        reflectivity = (hitIndex >= 0) ? spheres[hitIndex].reflectivity : 0.0f;
        if (reflectivity > 0.0f) {
            float reflectDir[3], offset[3], reflectOrigin[3], reflectColor[3];

            CgeVec3fReflect(direction, normal, reflectDir);
            CgeVec3fScale(reflectDir, 0.001f, offset);
            CgeVec3fAdd(hitPoint, offset, reflectOrigin);

            traceColor(reflectOrigin, reflectDir, reflectColor, depth + 1);

            CgeVec3fScale(localColor, (1.0f - reflectivity), localColor);
            CgeVec3fScale(reflectColor, reflectivity, reflectColor);
            CgeVec3fAdd(localColor, reflectColor, localColor);
        }

        memcpy(color, localColor, sizeof(localColor));
    } else {
        memcpy(color, bg, sizeof(bg));
    }
}

int main() {
    int x, y;
    float aspect;
    float direction[3], color[3];

    printf("P3\n%d %d\n255\n", WIDTH, HEIGHT);
    aspect = (float)WIDTH / (float)HEIGHT;

    for (y = 0; y < HEIGHT; y++) {
        for (x = 0; x < WIDTH; x++) {
            direction[0] = (2.0f * (x + 0.5f) / (float)WIDTH - 1.0f) * aspect;
            direction[1] = (2.0f * (HEIGHT - y - 0.5f) / (float)HEIGHT - 1.0f);
            direction[2] = -1.0;

            CgeVec3fNormal(direction, direction);
            traceColor(origin, direction, color, 0);
            CgeVec3fClamp(color, 0.0f, 1.0f, color);

            color[0] = powf(color[0], 1.0f / 2.2f);
            color[1] = powf(color[1], 1.0f / 2.2f);
            color[2] = powf(color[2], 1.0f / 2.2f);

            printf("%d %d %d\n", toInt(color[0]), toInt(color[1]), toInt(color[2]));
        }
    }

    return 0;
}
