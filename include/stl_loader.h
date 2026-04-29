#pragma once
#include <string>
#include <vector>
#include "mesh.h"

void initBounds(Bounds& b);
bool loadSTL(const std::string& path, std::vector<Triangle>& tris, Bounds& b);
