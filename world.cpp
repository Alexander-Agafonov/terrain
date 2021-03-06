#define GL_GLEXT_PROTOTYPES

#include <GL/glu.h>

#include <QTimer>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "world.h"

#define PGM_8_BIT 255
#define PGM_16_BIT 65535
#define PI_EXTERN 3.141592653589793238462643383279502884197169399375105820974944592307816406286


world :: world( QWidget* parent) : QGLWidget(parent) {

    setCursor(Qt::BlankCursor);
    setMouseTracking(true);

    this -> setFocusPolicy(Qt::StrongFocus);

    forward = false;
    back = false;
    left = false;
    right = false;
    up = false;
    down = false;

    cursor_x = -135;
    cursor_y = 110;

    sc_width = 0;
    sc_height = 0;

    fps = 0;
    fps_counter = 0;

    show_fps = false;

    connect(this, SIGNAL(exit_signal()), parentWidget(), SLOT(exit_scene()));

    camera = new fpp_camera(cursor_x, cursor_y);
    bad_terrain = new terrain();

}


void world :: resetFPS() {

    fps = fps_counter;
    fps_counter = 0;

}


// mouse
void world :: mouseMoveEvent(QMouseEvent* event) {

    QPoint mouse_position_change = (event -> globalPos() - mapToGlobal(rect().center()));

    if (mouse_position_change.x() != 0 || mouse_position_change.y() != 0) {

        cursor_x -= ((double) mouse_position_change.x() * 0.005);
        cursor_y += ((double) mouse_position_change.y() * 0.005);

        camera -> set_angles(cursor_x, cursor_y);
        cursor().setPos(mapToGlobal(rect().center()));
    }
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
            releaseKeyboard();
            releaseMouse();
            emit exit_signal();
            break;
        default:
            event->ignore();
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
    default:
        event->ignore();
        break;
    }
}


void world :: update_scene() { this -> repaint(); }


void world :: initializeGL() {

    glClearColor(1.f, 1.f, 1.f, 1.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    GLfloat light_pos[] = {0.f, 0.f, 1.f, 0.f};

    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);

    glMaterialfv(GL_FRONT, GL_DIFFUSE,   base.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  base.specular);
    glMaterialf( GL_FRONT, GL_SHININESS, base.shininess);
    glMaterialfv(GL_FRONT, GL_AMBIENT,   base.ambient);

    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);

}


void world :: resizeGL(int w, int h) {

    sc_width = w;
    sc_height = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-20 * ((double) w / h) * (1.f / sqrt(3)), 20 * ((double)w/h)* (1.f / sqrt(3)), -20 * (1.f / sqrt(3)), 20 * (1.f / sqrt(3)), 20, 2000.0);

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

    bad_terrain -> draw_terrain_arrays();

    // -fps
    if (bad_terrain -> dynamic && show_fps) {

        QPainter painter(this);
        painter.setPen(Qt :: black);
        if (bad_terrain -> current_cycle < bad_terrain -> cycles) painter.drawText(sc_width - 110, 20, "Iteration: " + QString :: fromStdString(std :: to_string(bad_terrain -> current_cycle + 1)) + " / " + QString :: fromStdString(std :: to_string(bad_terrain -> cycles)));
        else painter.drawText(sc_width - 60, 20, "Drying... ");
        painter.end();

    } else if (show_fps) {

          QPainter painter(this);
          painter.setPen(Qt :: black);
          painter.drawText(sc_width - 60, 20, "FPS: " + QString :: fromStdString(std :: to_string(fps)));
          painter.end();
    }

    glLoadIdentity();

    glFlush();

}
