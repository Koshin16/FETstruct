#pragma once
#include "mesh.h"

void drawMesh(const Mesh& m);
void drawText(float x, float y, float z, const char* s);
void drawTextSubscript(float x, float y, float z, const char* mainText, const char* subText);
void drawGroundSymbol(float x, float y, float z);
void drawPowerSupplySymbol(float x, float y, float z);
void drawFloor();
void drawShadow(const Mesh& m);
