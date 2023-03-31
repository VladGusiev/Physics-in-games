//CONTROLS:
//  Right arrow key / "1" -> SpeedUp
//  Left arrow key  / "2"  -> SpeedDown
//  Down arrow key  / "3" -> Stop

//drawAxes() was created for debugging purposes *could be turned on*

#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>

#define R 4

float time = 0;
float elapsedTime = 0;

float fit;
float fi0;

float wt;
float w0;

float vt;
float adt;
float att;

float e = 0.0f;

void update(const int iTime) {
    time = iTime * 0.001;
    float timeForFormula = time - elapsedTime;

    fit = fi0 + (w0 * timeForFormula) + (e * timeForFormula * timeForFormula) / 2;
    wt = w0 + (e * timeForFormula);

    vt = wt*R;
    adt = wt*wt*R;
    att = e*R;

    glutPostRedisplay();
    glutTimerFunc(1, update, iTime+1);
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(-10, 10, -10, 10,1,100);
    gluLookAt(5, 5, 5, 0.0f, 0.0f, 0.0f, 0, 1, 0);
}


void drawBall(void) {
    glLoadIdentity();

    glRotatef(fit, 0.0f, 0.0f, 1.0f);
    glTranslatef(R, 0.0f, 0.0f);
    glRotatef(90, 1 ,0, 0);

    glColor3f(1.0f, 1.0f, 0.0f);
    glutWireSphere(1, 20, 20);
}

void drawAxes() {
    glLoadIdentity();
    glBegin(GL_LINES);

    // X - Axis (Dark Red)
    glColor3f(0.19f, 0.02f, 0.02f);
    glVertex3f(-10, 0.0f, 0.0f);
    glVertex3f(10, 0.0f, 0.0f);

    // Y Axis (Dark Green)
    glColor3f(0.02f, 0.19f, 0.02f);
    glVertex3f(0.0f, -10, 0.0f);
    glVertex3f(0.0f, 10, 0.0f);

    // Z Axis (Dark Blue)
    glColor3f(0.02f, 0.02f, 0.19f);
    glVertex3f(0.0f, 0.0f, -10);
    glVertex3f(0.0f, 0.0f, 10);
    glEnd();
}

void drawTrajectory() {
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i <= 100; i++) {
        float a = 2.0f * M_PI * i / 100;
        float x = cosf(a)*R;
        float y = sinf(a)*R;
        glVertex2d(x, y);
    }
    glEnd();
}

void drawVectors() {
    //vt (BLUE)
    glLoadIdentity();
    glColor3f(0.0f, 0.0f, 1.0);
    glRotatef(fit, 0.0f, 0.0f, 1.0f);
    glTranslatef(R, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, vt, 0);
    glEnd();

    //adt (GREEN)
    glLoadIdentity();
    glColor3f(0.0f, 1.0f, 0.0f);
    glRotatef(fit, 0.0f, 0.0f, 1.0f);
    glTranslatef(R, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(-adt, 0, 0);
    glEnd();

    //att(RED)
    glLoadIdentity();
    glColor3f(1.0f, 0.0f, 0.0);
    glRotatef(fit, 0.0f, 0.0f, 1.0f);
    glTranslatef(R, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, att, 0);
    glEnd();

    //wt (WHITE)
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, wt);
    glEnd();
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    drawBall();
    drawAxes();
    drawTrajectory();
    drawVectors();

    glutSwapBuffers();
}

void recalculate() {
    elapsedTime = time;
    fi0 = fit;
    fit = 0;
    w0 = wt;
    wt = 0;
}

void keyboardNormalKey(unsigned char key, int x, int y)
{
    switch(key) {
        case 27: { //esc
            printf("w = %lf\n", e);
            exit(0);
        }
        case '1' : {
            e+=0.5f;
            recalculate();
            break;
        }
        case '2': {
            e-=0.5f;
            recalculate();
            break;
        }
        case '3' : {
            fi0 = fit;
            w0 = 0;
            e = 0;
            break;
        }
    }
}

void keyboardSpecialKey(int key , int x, int y)
{
    switch(key) {
        case GLUT_KEY_DOWN:
        {
            fi0 = fit;
            w0 = 0;
            e = 0;
            break;
        }
        case GLUT_KEY_LEFT:
        {
            e-=0.5f;
            recalculate();
            break;
        }
        case GLUT_KEY_RIGHT:
        {
            e+=0.5f;
            recalculate();
            break;
        }
    }
}



int main(int argc,char** argv)
{
    fi0 = 0.0f;
    w0 = 0.0f;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200 ,100);
    glutCreateWindow("Hello World"); // displaying window

    glutDisplayFunc(drawScene);

    glClearColor(0.21f, 0.15f, 0.26f, 0.0f); //background color

    glutReshapeFunc(handleResize);

    glutTimerFunc(1, update, 0);
    glutKeyboardFunc(keyboardNormalKey);
    glutSpecialFunc(keyboardSpecialKey);

    glutMainLoop();

    return 0;
}