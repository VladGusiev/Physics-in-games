#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

const int iTimeStep = 1; //in milliseconds (refresh rate)

float globalTime;

float g = 9.81f;

float initialSpeed;  //v0
float angle; // alpha

//position of a circle
float currentXPos; //x(t)
float currentYPos; //y(t)

float startingYPos = 0.0f; //h0

float xTransition; //vx(t)
float yTransition; //vy(t)

float sceneMaxY; //HH
float sceneMaxX; //LL

int arraySize = 100;

float* xCoords;
float* yCoords;

//only for display refresh
void objectsReposition(const int iHour) {
    globalTime = iHour;
    glutPostRedisplay();
    glutTimerFunc(iTimeStep, objectsReposition, iHour + 1);
}


void speedCalculations(int iTime) {

    float time = iTime*0.001;

    float cosAngle = cos(angle * (3.14f / 180.0f));
    float sinAngle = sin(angle * (3.14f / 180.0f));

    currentXPos = initialSpeed*cosAngle*time;
    currentYPos = startingYPos + (initialSpeed*sinAngle*time) - ((g*(time*time))/2);

    xTransition = initialSpeed*cosAngle;
    yTransition = (initialSpeed*sinAngle) - (g*time);

    if(iTime == arraySize) {
        arraySize *= 2;
        xCoords = realloc(xCoords, arraySize * sizeof(float));
        yCoords = realloc(yCoords, arraySize * sizeof(float));
    }

    xCoords[iTime] = currentXPos;
    yCoords[iTime] = currentYPos;

    //end
    if(currentYPos <= 0.0f) {
        free(xCoords);
        free(yCoords);
        return;
    }

    glutTimerFunc(1, speedCalculations, iTime + 1);
}

void sizeAdjustingService(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0.0, sceneMaxX*1.1, 0.0, sceneMaxY*1.1);
}

//drawing of objects
void objectsDraw() {
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    glTranslatef(currentXPos, currentYPos, 0);
    glColor3f(1.0, 0.0, 1.0);

    glBegin(GL_POLYGON);
    for (int i = 0; i <= 100; i++) {
        float a = 2.0f * 3.14f * i / 100;
        float x = cos(a);
        float y = sin(a);
        glVertex2d(x, y);
    }
    glEnd();
 // ----------------------------------------------------------- //
    glLoadIdentity();
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
        for(int i = 0; i < globalTime; i++){
            glVertex2f(xCoords[i], yCoords[i]);
        }
    glEnd();
 // ------------------------------------------------------------ //
    glLoadIdentity();
    glColor3f(0.0, 1.0, 1.0);
    glTranslatef(currentXPos, currentYPos, 0);
    glBegin(GL_LINES);
        glVertex2f(0, 0);
        glVertex2f(xTransition, yTransition);
    glEnd();
    glutSwapBuffers();
}

int main(int argc, char **argv) {


    int sceneSizeX;
    int sceneSizeY;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);

    initialSpeed = atof(argv[1]);
    angle = atof(argv[2]);
    startingYPos = atof(argv[3]);


    xCoords = (float*)malloc(arraySize*sizeof(float));
    yCoords = (float*)malloc(arraySize*sizeof(float));

    xCoords[0] = 0;
    yCoords[0] = startingYPos;


    float v0y = initialSpeed*sin(angle * (3.14f / 180.0f));
    float v0x = initialSpeed*cos(angle * (3.14f / 180.0f));

    sceneMaxY = startingYPos + (v0y*v0y)/(2*g);
    sceneMaxX = v0x*((v0y + sqrt((v0y*v0y) + 2*startingYPos*g))/g);

    if( sceneMaxX >= sceneMaxY) {
        sceneSizeX = 1080;
        sceneSizeY = sceneSizeX * sceneMaxY/sceneMaxX;
    } else {
        sceneSizeY = 640;
        sceneSizeX = sceneSizeY * (sceneMaxX/sceneMaxY);
    }

    glutInitWindowSize(sceneSizeX, sceneSizeY);
//    glutInitWindowPosition(300 ,300);

    glutCreateWindow("Hello World"); // displaying window

    glutDisplayFunc(objectsDraw); // calling function

    glClearColor(0.3, 0.2, 0.7, 0.0); //background color

    glutReshapeFunc(sizeAdjustingService);
    glutTimerFunc(iTimeStep, objectsReposition, 0);
    glutTimerFunc(1, speedCalculations, 0);

    glutMainLoop();

    free(xCoords);
    free(yCoords);
    return 0;
}
