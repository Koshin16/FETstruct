#include "camera.h"
#include <algorithm>

void resetCamera(Camera& c)
{
    c.rx = 22.0f;
    c.ry = -35.0f;
    c.zoom = -10.5f;
}

void initCamera(Camera& c)
{
    resetCamera(c);
    c.dragging = false;
    c.lastX = 0;
    c.lastY = 0;
}

void calcModelView(const Bounds& b, ModelView& m)
{
    m.center.x = (b.minx + b.maxx) * 0.5f;
    m.center.y = (b.miny + b.maxy) * 0.5f;
    m.center.z = (b.minz + b.maxz) * 0.5f;

    float dx = b.maxx - b.minx;
    float dy = b.maxy - b.miny;
    float dz = b.maxz - b.minz;
    float maxDim = std::max(dx, std::max(dy, dz));

    m.scale = (maxDim > 0.0f) ? 5.0f / maxDim : 1.0f;
}
