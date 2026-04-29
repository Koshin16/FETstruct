#pragma once
#include "mesh.h"

struct Camera {
    float rx, ry, zoom;
    bool dragging;
    int lastX, lastY;
};

struct ModelView {
    Vec3 center;
    float scale;
};

void resetCamera(Camera& c);
void initCamera(Camera& c);
void calcModelView(const Bounds& b, ModelView& m);
