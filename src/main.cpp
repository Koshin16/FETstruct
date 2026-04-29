/**
 * @file    main.cpp 
 * @brief   OFET 3D viewer (shadow toggle added)
 *
 * Keys:
 *   左ドラッグ : 回転
 *   ホイール / +/- : ズーム
 *   c : 配線 ON/OFF
 *   t : 文字 ON/OFF
 *   h : 影 ON/OFF  ← 追加
 *   s : PNG保存
 *   r : 視点リセット
 *   q / ESC : 終了
 */

#include <GL/glut.h>
#include <iostream>
#include <string>
#include <cstdlib>

#include "mesh.h"
#include "stl_loader.h"
#include "camera.h"
#include "draw_utils.h"
#include "screenshot.h"

Mesh gate, dielectricLayer, semiconductor, sourceElectrode, drainElectrode;
Bounds bounds;
ModelView view;
Camera cam;

bool showCircuit = true;
bool showText = true;
bool showShadow = true;   // ★追加

// =============================
// 配線描画
// =============================
void drawWiresAndLabels()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glLineWidth(5.0f);

    const float yWire = 2.50f;
    const float zWire = 1.12f;
    const float zGND  = -4.0f;

    glColor3f(0.0f,0.0f,0.0f);

    glBegin(GL_LINES);

    // Drain
    glVertex3f(0.55f, yWire, zWire);
    glVertex3f(-2.00f, yWire, zWire);
    glVertex3f(-2.00f, yWire, zWire);
    glVertex3f(-2.00f, yWire, zGND);

    // Source
    glVertex3f(7.45f, yWire, zWire);
    glVertex3f(9.20f, yWire, zWire);
    glVertex3f(9.20f, yWire, zWire);
    glVertex3f(9.20f, yWire, zGND);

    // Gate
    glVertex3f(4.00f, yWire, 0.00f);
    glVertex3f(4.00f, yWire, zGND);

    glEnd();

    drawGroundSymbol(-2.00f, yWire, zGND);
    drawPowerSupplySymbol(9.20f, yWire, -1.45f);
    drawGroundSymbol(9.20f, yWire, zGND);
    drawPowerSupplySymbol(4.00f, yWire, -1.45f);
    drawGroundSymbol(4.00f, yWire, zGND);

    if(showText){
        glDisable(GL_DEPTH_TEST);

        drawTextSubscript(9.85f, yWire, -0.65f, "V", "SD");
        drawTextSubscript(4.65f, yWire, -0.65f, "V", "G");
        drawText(-2.85f, yWire, zGND-0.70f, "GND");

        glEnable(GL_DEPTH_TEST);
    }

    glEnable(GL_LIGHTING);
}

// =============================
// デバイス描画
// =============================
void drawDevice()
{
    glDisable(GL_BLEND);
    glDepthMask(GL_TRUE);

    drawMesh(gate);
    drawMesh(semiconductor);
    drawMesh(sourceElectrode);
    drawMesh(drainElectrode);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    drawMesh(dielectricLayer);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
}

// =============================
// 影描画
// =============================
void drawDeviceShadow()
{
    glDisable(GL_LIGHTING);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    drawShadow(gate);
    drawShadow(dielectricLayer);
    drawShadow(semiconductor);
    drawShadow(sourceElectrode);
    drawShadow(drainElectrode);

    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_LIGHTING);
}

// =============================
// 描画本体
// =============================
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0, 0, cam.zoom);
    glRotatef(cam.rx, 1, 0, 0);
    glRotatef(cam.ry, 0, 0, 1);

    glScalef(view.scale, view.scale, view.scale);
    glTranslatef(-view.center.x, -view.center.y, -view.center.z);

    drawFloor();

    // ★ここが変更ポイント
    if(showShadow){
        drawDeviceShadow();
    }

    drawDevice();

    if(showCircuit){
        drawWiresAndLabels();
    }

    glutSwapBuffers();
}

// =============================
void reshape(int w,int h)
{
    if(h == 0) h = 1;

    glViewport(0,0,w,h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w/(double)h, 0.1, 1000.0);

    glMatrixMode(GL_MODELVIEW);
}

// =============================
void mouse(int button,int state,int x,int y)
{
    if(button == GLUT_LEFT_BUTTON){
        cam.dragging = (state == GLUT_DOWN);
        cam.lastX = x;
        cam.lastY = y;
    }

    if(state == GLUT_DOWN){
        if(button == 3) cam.zoom += 0.5f;
        if(button == 4) cam.zoom -= 0.5f;
    }

    glutPostRedisplay();
}

// =============================
void motion(int x,int y)
{
    if(cam.dragging){
        cam.ry += (x - cam.lastX) * 0.5f;
        cam.rx += (y - cam.lastY) * 0.5f;

        cam.lastX = x;
        cam.lastY = y;

        glutPostRedisplay();
    }
}

// =============================
void keyboard(unsigned char key,int,int)
{
    if(key == 'q' || key == 27) std::exit(0);

    if(key == '+') cam.zoom += 0.5f;
    if(key == '-') cam.zoom -= 0.5f;

    if(key == 'r') resetCamera(cam);
    if(key == 's') saveScreenshotPNG();

    if(key == 'c' || key == 'C'){
        showCircuit = !showCircuit;
        std::cout << "Circuit: " << (showCircuit ? "ON" : "OFF") << "\n";
    }

    if(key == 't' || key == 'T'){
        showText = !showText;
        std::cout << "Text: " << (showText ? "ON" : "OFF") << "\n";
    }

    // ★追加
    if(key == 'h' || key == 'H'){
        showShadow = !showShadow;
        std::cout << "Shadow: " << (showShadow ? "ON" : "OFF") << "\n";
    }

    glutPostRedisplay();
}

// =============================
void initGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);

    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

    GLfloat pos[]={5.0f,-6.0f,8.0f,1.0f};
    GLfloat dif[]={0.95f,0.95f,0.95f,1.0f};
    GLfloat amb[]={0.30f,0.30f,0.30f,1.0f};

    glLightfv(GL_LIGHT0,GL_POSITION,pos);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,dif);
    glLightfv(GL_LIGHT0,GL_AMBIENT,amb);

    glClearColor(0.93f,0.96f,1.0f,1.0f);
}

// =============================
bool loadDevice()
{
    const std::string base = "stl/";

    bool ok = true;
    ok &= loadSTL(base+"gate.stl", gate.tris, bounds);
    ok &= loadSTL(base+"dielectric.stl", dielectricLayer.tris, bounds);
    ok &= loadSTL(base+"semiconductor.stl", semiconductor.tris, bounds);
    ok &= loadSTL(base+"source.stl", sourceElectrode.tris, bounds);
    ok &= loadSTL(base+"drain.stl", drainElectrode.tris, bounds);

    return ok;
}

// =============================
void setColors()
{
    gate             = {gate.tris,             0.45f, 0.45f, 0.45f, 1.0f};
    dielectricLayer  = {dielectricLayer.tris,  0.25f, 0.55f, 1.00f, 0.38f};
    semiconductor    = {semiconductor.tris,    0.15f, 0.75f, 0.35f, 1.0f};
    sourceElectrode  = {sourceElectrode.tris,  1.00f, 0.78f, 0.05f, 1.0f};
    drainElectrode   = {drainElectrode.tris,   1.00f, 0.78f, 0.05f, 1.0f};
}

// =============================
int main(int argc,char** argv)
{
    initBounds(bounds);
    initCamera(cam);

    if(!loadDevice()){
        std::cerr << "STL loading failed.\n";
        return 1;
    }

    setColors();
    calcModelView(bounds, view);

    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000,700);
    glutCreateWindow("OFET Viewer");

    initGL();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}