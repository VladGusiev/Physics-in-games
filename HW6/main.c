#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <math.h>
#include <stdio.h>
//TODO 2. without arguments start settings

struct trajectory {
    float* xCoords;
    float* yCoords;
    float* zCoords;

    int size;
    int capacity;
};

struct sphere {
    float startingHeight;

    float currentXPos, currentYPos, currentZPos; //x,y,z (t)
    float xTransition, yTransition, zTransition; //vx,vy (t)

    float xSpeed, ySpeed, zSpeed, xzSpeed;
    float initalSpeed;

    float alpha;
    float azimuthAngle;
    struct trajectory trajectory;
};

struct sphere ball;

void speeedPositionCalculations();

float time = 0;
int globalTime;

float g = 9.81f;

void update(const int iTime) {
    time = iTime * 0.001;
    globalTime = iTime;

    speeedPositionCalculations();
    if(ball.currentYPos <= 0) return;

    glutPostRedisplay();
    glutTimerFunc(1, update, iTime+1);
}

void speeedPositionCalculations() {
    float sinAlpha = sinf(ball.alpha*(M_PI/180.0f));
    float cosAlpha = cosf(ball.alpha*(M_PI/180.0f));

    float cosAzimuth = cosf(ball.azimuthAngle*(M_PI/180.0f));
    float sinAzimuth = sinf(ball.azimuthAngle*(M_PI/180.0f));

    ball.xzSpeed = ball.initalSpeed*cosAlpha;
    ball.xSpeed = ball.xzSpeed*cosAzimuth;
    ball.zSpeed =  ball.xzSpeed*sinAzimuth;
    ball.ySpeed = ball.initalSpeed*sinAlpha;

    ball.currentXPos = ball.xSpeed*time;
    ball.currentYPos = ball.startingHeight + (ball.ySpeed*time) - ((g*time*time)/2);
    ball.currentZPos = ball.zSpeed*time;

    ball.xTransition = ball.xSpeed;
    ball.yTransition = ball.ySpeed - (g*time);
    ball.zTransition = ball.zSpeed;
}

void handleResize(int w, int h) {
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(-30, 30, -30, 30,1,100);
    gluLookAt(30, 30, 30, 0.0f, 0.0f, 0.0f, 0, 1, 0);
}

void drawTrajectory() {
    if(globalTime == ball.trajectory.size) {
        ball.trajectory.size *= 2;

        ball.trajectory.xCoords = realloc(ball.trajectory.xCoords, ball.trajectory.size * sizeof(float));
        ball.trajectory.yCoords = realloc(ball.trajectory.yCoords, ball.trajectory.size * sizeof(float));
        ball.trajectory.zCoords = realloc(ball.trajectory.zCoords, ball.trajectory.size * sizeof(float));
    }

    ball.trajectory.xCoords[globalTime] = ball.currentXPos;
    ball.trajectory.yCoords[globalTime] = ball.currentYPos;
    ball.trajectory.zCoords[globalTime] = ball.currentZPos;

    glLoadIdentity();
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i < globalTime; i++){
        glVertex3f(ball.trajectory.xCoords[i], ball.trajectory.yCoords[i], ball.trajectory.zCoords[i]);
    }
    glEnd();
}

void drawVector() {
    glLoadIdentity();
    glColor3f(0.0, 1.0, 1.0);
    glTranslatef(ball.currentXPos, ball.currentYPos, ball.currentZPos);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    glVertex3f(ball.xTransition, ball.yTransition, ball.zTransition);
    glEnd();
}

void drawBall(void) {
    glLoadIdentity();

    glTranslatef(ball.currentXPos, ball.currentYPos, ball.currentZPos);

    glColor3f(1.0f, 1.0f, 0.0f);
    glutWireSphere(1, 10, 10);

    drawTrajectory();
    drawVector();
}

void drawAxes() {
    glLoadIdentity();
    glBegin(GL_LINES);

    // X - Axis (Dark Red)
    glColor3f(0.19f, 0.02f, 0.02f);
    glVertex3f(-30, 0.0f, 0.0f);
    glVertex3f(30, 0.0f, 0.0f);

    // Y Axis (Dark Green)
    glColor3f(0.02f, 0.19f, 0.02f);
    glVertex3f(0.0f, 30, 0.0f);
    glVertex3f(0.0f, 0, 0.0f);

    // Z Axis (Dark Blue)
    glColor3f(0.02f, 0.02f, 0.19f);
    glVertex3f(0.0f, 0.0f, -30);
    glVertex3f(0.0f, 0.0f, 30);
    glEnd();
}

void drawShadow() {
    glLoadIdentity();
    glTranslatef(ball.currentXPos, 0, ball.currentZPos);
    glColor3f(0, 0, 0);

    glBegin(GL_POLYGON);
    for (int i = 0; i <= 100; i++) {
        float a = 2.0f * 3.14f * i / 100;
        float x = cosf(a);
        float z = sinf(a);
        glVertex3f(x, -1, z);
    }
    glEnd();
}

void isEnd() {
    if(ball.currentYPos <= 0.0f) {
        free(ball.trajectory.xCoords);
        free(ball.trajectory.yCoords);
        free(ball.trajectory.zCoords);
        return;
    }
}

void drawScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    drawAxes();
    drawShadow();
    drawBall();
//    drawTrajectory();
//    drawVectors();
    isEnd();

    glutSwapBuffers();
}

int main(int argc,char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE);
    glEnable(GL_DEPTH_TEST);


    ball.trajectory.size = 100;
    ball.trajectory.xCoords = (float*)malloc(ball.trajectory.size*sizeof(float));
    ball.trajectory.yCoords = (float*)malloc(ball.trajectory.size*sizeof(float));
    ball.trajectory.zCoords = (float*)malloc(ball.trajectory.size*sizeof(float));

    ball.trajectory.xCoords[0] = 0;
    ball.trajectory.yCoords[0] = ball.startingHeight;
    ball.trajectory.zCoords[0] = 0;

    ball.startingHeight = atof(argv[1]);
    ball.initalSpeed = atof(argv[2]);
    ball.alpha = atof(argv[3]);
    ball.azimuthAngle = atof(argv[4]);

    glutInitWindowSize(800, 600);
    glutInitWindowPosition(200 ,100);
    glutCreateWindow("Hello World"); // displaying window

    glutDisplayFunc(drawScene);

    glClearColor(0.21f, 0.15f, 0.26f, 0.0f); //background color

    glutReshapeFunc(handleResize);

    glutTimerFunc(1, update, 0);

    glutMainLoop();

    free(ball.trajectory.xCoords);
    free(ball.trajectory.yCoords);
    free(ball.trajectory.zCoords);

    return 0;
}