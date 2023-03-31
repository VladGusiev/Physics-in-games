#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

const int iTimeStep = 1; //in milliseconds (refresh rate)
const float Lmax = 50.0; //scene size x-coord

float circ1InitialSpeed;  //v01
float circ2InitialSpeed;  //v02

float circ1Acc;  //a01
float circ2Acc;  //a02

//starting pos
float circ1StartingPos = 0.0;
float circ2StartingPos = 0.0;


float circ1CurrentPos;
float circ2CurrentPos;

float circ1CurrentTransition; // x1(t)
float circ2CurrentTransition; // x2(t)

float circ1CurrentSpeed; // v1(t)
float circ2CurrentSpeed; // v2(t)

char *filename = "output.txt";

//only for display refresh
void actualization(const int iHour) {
    glutPostRedisplay();
    glutTimerFunc(iTimeStep, actualization, iHour+10);
}


//all calculations
void timeFunc(const int iTime) {

    float time = iTime*0.001;

    circ1CurrentTransition = (circ1InitialSpeed * time) + ((circ1Acc * time * time) / 2);
    circ2CurrentTransition = (circ2InitialSpeed * time) + ((circ2Acc * time * time) / 2);

    circ1CurrentSpeed = circ1InitialSpeed + (circ1Acc*time);
    circ2CurrentSpeed = circ2InitialSpeed + (circ2Acc*time);

    circ1CurrentPos = circ1StartingPos + circ1CurrentTransition;
    circ2CurrentPos = circ2StartingPos + circ2CurrentTransition;

    FILE *fp = fopen(filename, "a+");

    if(fp == NULL) {
        printf("ERROR while opening file");
        return;
    }

    fprintf(fp, "%f ",time);
    fprintf(fp, "%f ",circ1CurrentTransition);

    fprintf(fp, "%f ",circ1CurrentSpeed);
    fprintf(fp, "%f ",circ2CurrentTransition);
    fprintf(fp, "%f",circ2CurrentSpeed);
    fprintf(fp, "\n");

    fclose(fp);

    //stop when they meet or when 30 seconds pass
    if(circ1CurrentPos >= circ2CurrentPos || time > 30) {
        printf("%f", time);
        return;
    }
    glutTimerFunc(1, timeFunc, iTime+1);
}

void sizeAdjustingService(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);

    if (width == 0) width++;
    const float sizeProportion = ((float)height)/width;

    //range: <-1, 1>
    gluOrtho2D(-0.5*Lmax, 0.5*Lmax, -0.5*Lmax*sizeProportion, 0.5*Lmax*sizeProportion);
}

//drawing of objects
void objectsDraw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslatef(circ1CurrentPos, 0, 0);
    glColor3f(1.0, 0.0, 1.0);
    glBegin(GL_POLYGON);
//    int c = 100;
    for (int i = 0; i <= 100; i++) {
        float a = 2 * 3.14 * i / 100;
        float x = cos(a);
        float y = sin(a);
        glVertex2d(x, y);
    }
    glEnd();

//----------------------------------------------------------------------//
    glLoadIdentity();
    glColor3f(0.0, 0.0, 1.0);
    glTranslatef(circ2CurrentPos, 0, 0);
    glBegin(GL_POLYGON);
//    int c = 100;
    for (int i = 0; i <= 100; i++) {
        float a = 2 * 3.14 * i / 100;
        float x = cos(a);
        float y = sin(a);
        glVertex2d(x, y);
    }
    glEnd();
// ---------------------------------------------------------------------//
    glutSwapBuffers();
}

int main(int argc, char **argv) {
    FILE *fp = fopen(filename, "w");
    fclose(fp);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);

    glutInitWindowSize(800, 450);
    glutInitWindowPosition(300 ,300);
    glutCreateWindow("Hello World"); // displaying window

    float startingCoord = atof(argv[1]);

    circ1InitialSpeed = atof(argv[2]);
    circ1Acc = atof(argv[3]);

    circ2InitialSpeed = atof(argv[4]);
    circ2Acc = atof(argv[5]);

    circ1StartingPos = 0;
    circ2StartingPos = startingCoord;

    glutDisplayFunc(objectsDraw); // calling function

    glClearColor(0.3, 0.2, 0.7, 0.0); //background color

    glutReshapeFunc(sizeAdjustingService);

    glutTimerFunc(iTimeStep, actualization, 0);
    glutTimerFunc(1, timeFunc, 0);
    glutMainLoop();

    return 0;
}
