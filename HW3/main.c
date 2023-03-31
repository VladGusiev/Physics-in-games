#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>

const int iTimeStep = 1; //in milliseconds (refresh rate)
const float Lmax = 50.0;

int isGoingUp = 1;

float globalTime;

float roofHeight;
int touchedRoof = 0;

float g = 9.81f;
float k;

float circ1InitialSpeed;  //v01

//starting pos
float circ1StartingPos = 0.0f;

float circ1CurrentPos; //y(t)

float circ1CurrentTransition; // offset

float circ1CurrentSpeed; // v1(t)

char *filename = "output.txt";

//only for display refresh
void actualization(const int iHour) {
    globalTime = iHour*0.001;
    glutPostRedisplay();
    glutTimerFunc(iTimeStep, actualization, iHour+1);
}

//all calculations
void timeFunc(int iTime) {

    float time = iTime*0.001;

    circ1CurrentPos = circ1StartingPos + circ1CurrentTransition;

    // going down
    if(isGoingUp == 0) {
        if(circ1CurrentPos <= 0.0f) {

            circ1CurrentTransition =  roofHeight - (circ1InitialSpeed*time) - ((g*(time*time)) / 2);
            circ1CurrentSpeed = circ1InitialSpeed + (g*time);

            circ1InitialSpeed = circ1CurrentSpeed*k;

            isGoingUp = 1;
            iTime = 0;
        } else {
            circ1CurrentTransition =  roofHeight - (circ1InitialSpeed*time) - ((g*(time*time)) / 2);
            circ1CurrentSpeed = circ1InitialSpeed + (g*time);
        }
    } else {
        //going up
        if (circ1CurrentPos >= roofHeight) {
            touchedRoof++;

            circ1CurrentTransition = (circ1InitialSpeed * time) - ((g * (time * time)) / 2);
            circ1CurrentSpeed = circ1InitialSpeed - (g*time);

            circ1InitialSpeed = circ1CurrentSpeed*k;

            isGoingUp = 0;
            iTime = 0;
        } else {
            circ1CurrentTransition = (circ1InitialSpeed * time) - ((g * (time * time)) / 2);
            circ1CurrentSpeed = circ1InitialSpeed - (g*time);
        }
    }

    FILE *fp = fopen(filename, "a+");

    if(fp == NULL) {
        printf("ERROR while opening file");
        return;
    }

    fprintf(fp, "%f ",globalTime);
    fprintf(fp, "%f ",circ1CurrentPos);
    fprintf(fp, "%f ",circ1CurrentSpeed);
    fprintf(fp, "\n");

    fclose(fp);

    if(circ1CurrentSpeed <= 0.0f) {
        float v1 = sqrtf(2*g*roofHeight*(1-(1/(k*k))+(1/(k*k*k*k))));
        printf("Touched roof: %d times\n", touchedRoof);
        printf("v1 = %f\n-------------", v1);
        return;
    }

    glutTimerFunc(1, timeFunc, iTime+1);
}

void sizeAdjustingService(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);

    if (width == 0) width++;
    const float sizeProportion = ((float)height)/width;

    gluOrtho2D(-0.5*Lmax, 0.5*Lmax, -0.5*Lmax*sizeProportion, 0.5*Lmax*sizeProportion);
}

//drawing of objects
void objectsDraw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslatef(0, circ1CurrentPos, 0);
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
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex2f(-25, roofHeight);
        glVertex2f(25, roofHeight);
    glEnd();
// ---------------------------------------------------------------------//
    glLoadIdentity();
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
        glVertex2f(-25, 0);
        glVertex2f(25, 0);
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

    roofHeight = atof(argv[1]);

    circ1InitialSpeed = atof(argv[2]);

    k = atof(argv[3]);

    circ1StartingPos = 0;

    glutDisplayFunc(objectsDraw); // calling function

    glClearColor(0.3, 0.2, 0.7, 0.0); //background color

    glutReshapeFunc(sizeAdjustingService);

    glutTimerFunc(iTimeStep, actualization, 0);
    glutTimerFunc(1, timeFunc, 0);
    glutMainLoop();

    return 0;
}
