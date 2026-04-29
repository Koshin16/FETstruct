#include "stl_loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>

void initBounds(Bounds& b)
{
    b.minx = b.miny = b.minz =  1e9f;
    b.maxx = b.maxy = b.maxz = -1e9f;
}

static void updateBounds(Bounds& b, const Vec3& p)
{
    b.minx = std::min(b.minx, p.x); b.maxx = std::max(b.maxx, p.x);
    b.miny = std::min(b.miny, p.y); b.maxy = std::max(b.maxy, p.y);
    b.minz = std::min(b.minz, p.z); b.maxz = std::max(b.maxz, p.z);
}

bool loadSTL(const std::string& path, std::vector<Triangle>& tris, Bounds& b)
{
    std::ifstream f(path);
    if(!f){
        std::cerr << "Failed to open: " << path << "\n";
        return false;
    }

    std::string word;
    Triangle t{};
    int v = 0;

    while(f >> word){
        if(word == "facet"){
            f >> word; // normal
            f >> t.n.x >> t.n.y >> t.n.z;
        }
        else if(word == "vertex"){
            f >> t.v[v].x >> t.v[v].y >> t.v[v].z;
            updateBounds(b, t.v[v]);
            v++;
            if(v == 3){
                tris.push_back(t);
                v = 0;
            }
        }
    }

    std::cout << path << " : " << tris.size() << " triangles\n";
    return !tris.empty();
}
