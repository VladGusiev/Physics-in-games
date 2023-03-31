#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>


const int iTimeStep = 100; //in milliseconds (refresh rate)
const float Lmax = 20.0; //scene size x-coord

float move1TriangleX = 0.0; // start x-coord of 1 object

float moveRectangle2XY = 0.0; // start x-coord of 2nd object

float moveRectangle3X = 1.0; //start x-coord of 3rd object
float moveRectangle3Y = 0.0; //start y-coord of 3rd object

float moveTriangByAmount = -0.025;
float rec3XSpeed = 0.05;
float rec3YSpeed = 0.0;

int lastXSign = 1;

void actualization(const int iHour) {


    if((iHour % 300) == 0) {
        moveTriangByAmount *= -1;

    }

    if((iHour % 200) == 0) {
        if(rec3XSpeed == 0) {
            if (lastXSign == 1) {
                rec3XSpeed = -0.05;
                rec3YSpeed = 0.0;
                lastXSign = 0;
            } else {
                rec3XSpeed = 0.05;
                rec3YSpeed = 0.0;
                lastXSign = 1;
            }
        } else {
            if (lastXSign == 1) {
                rec3XSpeed = 0.0;
                rec3YSpeed = 0.05;
                lastXSign = 1;
            } else {
                rec3XSpeed = 0;
                rec3YSpeed = -0.05;
                lastXSign = 0;
            }
        }
    }
    moveRectangle3X += rec3XSpeed;
    moveRectangle3Y += rec3YSpeed;
    moveRectangle2XY += 0.05;
    move1TriangleX += moveTriangByAmount;

    glutPostRedisplay();

    glutTimerFunc(iTimeStep, actualization, iHour+10);

}

void sizeAdjustingService(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);

    if (width == 0) width++;
    const float sizeProportion = ((float)height)/width;

    //range: <-1, 1>
    gluOrtho2D(-0.5*Lmax, 0.5*Lmax, -0.5*Lmax*sizeProportion, 0.5*Lmax*sizeProportion);
}

void objectsDraw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0, 0.0, 1.0);
    glTranslatef(moveRectangle2XY, moveRectangle2XY, 0.0);

    glBegin(GL_QUADS);
    glVertex2f(0, 0); //left top
    glVertex2f(1.0, 0); //right top
    glVertex2f(1.0, 1.0); //right down
    glVertex2f(0, 1.0); //left down
    glEnd();
//----------------------------------------------------------------------//
    glColor3f(0.0, 0.0, 1.0);
    glLoadIdentity();
    glTranslatef(moveRectangle3X, moveRectangle3Y, 0);
    glRectf(0.0, 0.0, 1, 1);
// ---------------------------------------------------------------------//
    glColor3f(0.0, 1.0, 0.0); //TRIANGLE color
    glLoadIdentity();
    glTranslatef(move1TriangleX, 0.0, 0.0);

    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(1.0, 0);
    glVertex2f(0.5, 1.0);
    glEnd();

    glutSwapBuffers();
}

int main(int argc, char **argv) {

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(800, 450);
    glutInitWindowPosition(100 ,100);
    glutCreateWindow("Hello World"); // displaying window

    glutDisplayFunc(objectsDraw); // calling function

    glClearColor(0.7, 0.0, 0.7, 0.0); //background color

    glutReshapeFunc(sizeAdjustingService);

    glutTimerFunc(iTimeStep, actualization, 0);
    glutMainLoop();


    return 0;
}
