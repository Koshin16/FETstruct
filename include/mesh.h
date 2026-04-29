#pragma once
#include <vector>

struct Vec3 { float x, y, z; };

struct Triangle {
    Vec3 n;
    Vec3 v[3];
};

struct Mesh {
    std::vector<Triangle> tris;
    float r, g, b, a;
};

struct Bounds {
    float minx, miny, minz;
    float maxx, maxy, maxz;
};
