#include "draw_utils.h"
#include <GL/glut.h>

void drawMesh(const Mesh& m)
{
    if(m.tris.empty()) return;

    glColor4f(m.r, m.g, m.b, m.a);
    glBegin(GL_TRIANGLES);
    for(const auto& t : m.tris){
        glNormal3f(t.n.x, t.n.y, t.n.z);
        for(int i=0; i<3; i++){
            glVertex3f(t.v[i].x, t.v[i].y, t.v[i].z);
        }
    }
    glEnd();
}

void drawText(float x, float y, float z, const char* s)
{
    glRasterPos3f(x, y, z);
    for(const char* p=s; *p; ++p){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }
}

void drawTextSubscript(float x, float y, float z, const char* mainText, const char* subText)
{
    glRasterPos3f(x, y, z);
    for(const char* p=mainText; *p; ++p){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
    }

    glRasterPos3f(x+0.23f, y, z-0.22f);
    for(const char* p=subText; *p; ++p){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
    }
}

void drawGroundSymbol(float x, float y, float z)
{
    const float w = 0.45f;
    glBegin(GL_LINES);
    glVertex3f(x-w, y, z); glVertex3f(x+w, y, z);
    glVertex3f(x-w*0.70f, y, z-0.20f); glVertex3f(x+w*0.70f, y, z-0.20f);
    glVertex3f(x-w*0.40f, y, z-0.40f); glVertex3f(x+w*0.40f, y, z-0.40f);
    glEnd();
}

void drawPowerSupplySymbol(float x, float y, float z)
{
    glBegin(GL_LINES);
    glVertex3f(x-0.35f,y,z); glVertex3f(x+0.35f,y,z);
    glVertex3f(x-0.20f,y,z+0.28f); glVertex3f(x+0.20f,y,z+0.28f);

    // variable arrow
    glVertex3f(x-0.50f,y,z-0.25f); glVertex3f(x+0.50f,y,z+0.55f);
    glVertex3f(x+0.50f,y,z+0.55f); glVertex3f(x+0.25f,y,z+0.48f);
    glVertex3f(x+0.50f,y,z+0.55f); glVertex3f(x+0.42f,y,z+0.30f);
    glEnd();
}

void drawFloor()
{
    glDisable(GL_LIGHTING);
    glColor3f(0.86f, 0.86f, 0.86f);

    glBegin(GL_QUADS);
    glVertex3f(-10.0f,-10.0f,-4.35f);
    glVertex3f( 12.0f,-10.0f,-4.35f);
    glVertex3f( 12.0f, 10.0f,-4.35f);
    glVertex3f(-10.0f, 10.0f,-4.35f);
    glEnd();

    glEnable(GL_LIGHTING);
}

void drawShadow(const Mesh& m)
{
    if(m.tris.empty()) return;

    const float zShadow = -4.28f;
    const float sx = 0.25f;
    const float sy = -0.20f;

    glColor4f(0.0f, 0.0f, 0.0f, 0.18f);

    glBegin(GL_TRIANGLES);
    for(const auto& t : m.tris){
        for(int i=0; i<3; i++){
            float dz = t.v[i].z - zShadow;
            glVertex3f(t.v[i].x + sx*dz, t.v[i].y + sy*dz, zShadow);
        }
    }
    glEnd();
}
