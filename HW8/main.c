#include <stdio.h>
#include <math.h>


// given
float g  = 9.81f;  // m/s
float R  = 8.314f; // J / (mol*s)
float P = 101325; // Pa
float M  = 0.029f; // kg / mol
float S  = 2;      // m*m
float C  = 0.5f;   //
float m  = 80;    //kg

float maxTime = 180;

float K; // declared in main
float v0 = 0;
float vt;
float zt;
float vInf; // declared in main
float z0 = 1000;
float p = 1;
float T = 295;
float e = 2.71f; // euler

// analytical
void firstMethod() {
    FILE *f = fopen("./firstMethod.txt", "w");

    float dt = 0.01f; //delta time
    float t = 0.0f;    //current time

    while (t < maxTime) {
        t += dt;

        vt = (v0 - (vInf * tanhf((g * t) / vInf))) / (1.0f - ((v0 / vInf) * tanhf((g * t) / vInf)));

        zt = z0 - ((powf(vInf, 2) / g) * logf(coshf((g * t) / vInf)));

        if (zt <= 0) break;
        fprintf(f, "%f %f %f\n", t, vt >= 0 ? vt : -vt, zt);
    }
    fclose(f);
}

//Euler
void secondMethod() {
    float dt = 0.1f; //TODO here change delta time to "1" / ".01";
    FILE *f = fopen("./secondMethod.txt", "w");

    float t = 0;

    float vFollowing = 0.0f;
    float zFollowing = z0;

    while (t < maxTime) {
        t += dt;

        vt = vFollowing;
        vFollowing = vt + dt * (-g - K * fabsf(vt) * vt);
        zt = zFollowing;
        zFollowing = zt + dt * vt;

        if(zt <= 0) break;
        fprintf(f, "%f %f %f\n", t, vt >= 0 ? vt : -vt, zt);
    }
    fclose(f);
}

// second Euler method
void thirdMethod() {
    FILE *f = fopen("./thirdMethod.txt", "w");

    float dt = 0.01f;

    float p0 = (M / (R * T)) * P;
    float k = (M * g) / (R * T);
    float K0 = 0.5f * ((C * S * p0) / m);

    float t = 0.0f;
    float vFollowing = 0.0f;
    zt = z0;
    float zFollowing = z0;

    while (t < maxTime) {
        t += dt;

        p = p0 * powf(e, -k * zt);

        vt = vFollowing;
        vFollowing = vt + dt * (-g - K0 * powf(e, -k * zt) * fabsf(vt) * vt);

        zt = zFollowing;
        zFollowing = zt + dt * vt;

        if (zt <= 0) break;

        fprintf(f, "%f %f %f\n", t, vt >= 0 ? vt : -vt, zt);
    }

    fclose(f);
}
int main() {
    //initial declaration
    K = 0.5f * ((C * S * p) / m);
    vInf = sqrtf((2.0f * m * g) / (p * C * S));

    firstMethod();
    secondMethod();
    thirdMethod();

    return 0;
}
