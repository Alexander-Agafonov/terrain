
#define GL_GLEXT_PROTOTYPES

//#include <GL/GL.h>
#include <GL/glu.h>

#include <QTimer>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


#include "world.h"
#include "noise.h"


#define PGM_8_BIT 255
#define PGM_16_BIT 65535
#define PI_EXTERN 3.141592653589793238462643383279502884197169399375105820974944592307816406286


world :: world( QWidget* parent) : QGLWidget(parent) {

    tick = 0;
    zoom = 0;
    fps = 0;
    fps_counter = 0;
    show_fps = true;
    loaded =false;
    sc_width = 0;
    sc_height = 0;

    setCursor(Qt::BlankCursor);
    setMouseTracking(true);

    this->setFocusPolicy(Qt::StrongFocus);
    //this -> grabKeyboard();


    //painter = new QPainter(this);

//    grabMouse();

    direction_x = 0;
    direction_y = 0;
    direction_z = 0;

    forward = false;
    back = false;
    left = false;
    right = false;
    up = false;
    down = false;
    menu = false;

    connect(this, SIGNAL(toggle_menu_signal()), parentWidget(), SLOT(toggle_scene_menu()));
//    connect(this, SIGNAL(scene_ready_signal()), parentWidget(), SLOT(scene_ready()));
//    connect(this, SIGNAL(current_stage_signal()), this -> splash, SLOT(update_stage()));


          //painter = new QPainter(this);



    cx = 0;
    cy = 0;
    cz = 0;

    noisy = new noise();

    cursor_x = -135;
    cursor_y = 110;
    camera = new fpp_camera(cursor_x, cursor_y);

    terrarray = new int[10000];

    noisy -> black_noise();
    //this -> black_noise();

    mouse_prev_pos_x = 0;
    mouse_prev_pos_y = 0;


    camera_position_x = 0;
    camera_position_y = 0;
    camera_position_z = 0;


    QPoint n;
    mouse_position = n;

    shake = 0;
    shake_right = true;
    jump = false;

    this -> terra = new terrain();

}

void world :: cameraUpdate(double x, double y, double z) {
    zoom += 1;

}

void world :: emit_toggle_menu_signal() { emit toggle_menu_signal(); }

//void world :: emit_scene_ready_signal() { emit scene_ready_signal(); }




void world :: resetFPS() {
    std :: cout << fps << "\n";
    fps = fps_counter;
    fps_counter = 0;
}


// mouse
void world :: mouseMoveEvent(QMouseEvent* event) {

    QPoint mouse_position_change = (event -> globalPos() - mapToGlobal(rect().center()));

    if (mouse_position_change.x() != 0 && mouse_position_change.y() != 0) {

        cursor_x -= ((double) mouse_position_change.x() * 0.005);
        cursor_y += ((double) mouse_position_change.y() * 0.005);

        camera -> set_angles(cursor_x, cursor_y);

        cursor().setPos(mapToGlobal(rect().center()));
    }
}


void world :: mousePressEvent(QMouseEvent* event) {

}

// keyboard
void world :: keyPressEvent(QKeyEvent* event) {

    if (event -> isAutoRepeat()) return;
    switch (event -> key()) {
        case Qt :: Key_W:
            forward = true;
            break;
        case Qt :: Key_S:
            back = true;
            break;
        case Qt :: Key_A:
            left = true;
            break;
        case Qt :: Key_D:
            right = true;
            break;
        case Qt :: Key_Space:
            up = true;
            break;
        case Qt :: Key_Shift:
            down = true;
            break;

    case Qt :: Key_F:
        if (show_fps) show_fps = false;
        else show_fps = true;
        break;
        case Qt :: Key_Escape:
            emit_toggle_menu_signal();
            break;
    }
}

void world :: keyReleaseEvent(QKeyEvent* event) {

    if (event -> isAutoRepeat()) return;
    switch (event -> key()) {
        case Qt :: Key_W:
            forward = false;
            break;
        case Qt :: Key_S:
            back = false;
            break;
        case Qt :: Key_A:
            left = false;
            break;
        case Qt :: Key_D:
            right = false;
            break;
        case Qt :: Key_Space:
            up = false;
            break;
        case Qt :: Key_Shift:
            down = false;
            break;
    }
}

void world :: update_scene() {
    tick++;
    this -> repaint();
}

void world :: reload() {

    if (terrain_mx.size() > 0) for (int i = 0; i < terrain_mx.size(); i++) terrain_mx[i].clear();
    terrain_mx.clear();
//    terrain_mx = terra -> load_terrain();

}


static const float PI = 3.1415926535;

void world :: initializeGL() {

    glClearColor(1.f, 1.f, 1.f, 1.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_FOG);

    glFogi(GL_FOG_MODE, GL_LINEAR);
    glFogf(GL_FOG_START, 200.f);
    glFogf(GL_FOG_END, 2000.f);

    float fcolour[3]={0.4, 0.7, 0.9};

    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glFogfv(GL_FOG_COLOR, fcolour);
    glHint(GL_FOG_HINT, GL_FASTEST);
    glFogf(GL_FOG_DENSITY,1.f);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_pos[] = {0., 0., 1., 0.};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    reload();

    //float positions[9] = { -100, -100, 0.5, 100, -100, 0.5, 100, 100, 0.5 };
    float colors[9] = { 1,0,0,  0,1,0,  1,0,0 };


    glMaterialfv(GL_FRONT, GL_DIFFUSE,   base.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  base.specular);
    glMaterialf( GL_FRONT, GL_SHININESS, base.shininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT,   base.ambient);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);


    //glColorPointer( 3, GL_FLOAT, 0, colors );

  // Enable use of arrays.
   // glEnableClientState( GL_COLOR_ARRAY );


   // glEnableClientState(GL_VERTEX_ARRAY);


    //load_terrain();
 //emit_scene_ready_signal();



       terra -> load_terrain();


}


void world :: resizeGL(int w, int h) {

    sc_width = w;
    sc_height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-20 * ((double)w/h) * (1. / sqrt(3)), 20 * ((double)w/h)* (1. / sqrt(3)), -20* (1. / sqrt(3)), 20* (1. / sqrt(3)), 20, 2000.0);

}


void world :: paintGL() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_NORMALIZE);
    glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glLoadIdentity();

    fps_counter++;

    camera -> move_camera(forward, back, left, right, up, down);
    camera -> look();

              // look from
    gluLookAt(camera -> position_x, camera -> position_y, camera -> position_z ,
              // look at
              camera -> look_at_x, camera -> look_at_y, camera -> look_at_z,
              // camera rotation
              0., 0., 1.);

    terra -> draw_terrain_arrays();

    glLoadIdentity();

    glFlush();

    // eats fps
    if (show_fps) {
          QPainter painter(this);
          painter.setPen(Qt :: black);
          painter.drawText(sc_width - 60, 20, "FPS: " + QString :: fromStdString(std :: to_string(fps)));
          painter.end(); }

}
