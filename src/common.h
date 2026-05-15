#ifndef COMMON_H
#define COMMON_H

#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif

#include <math.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N 12
#define SIZE 2.0f

typedef struct {
    float X, Y, Z;
    float x, y, z;
    float u, v, w;
    float delta_T;
    float strain_vm;
    float stress_vm;
    float energy;
    float eta;
} Point;

extern float E;
extern float nu;
extern float alpha;
extern float c_v;
extern float T_0;
extern float lambda_lame, mu_lame;
extern Point grid[N][N][N];
extern float delta_L;

extern int selected_i, selected_j, selected_k;
extern int last_mouse_x, last_mouse_y;
extern int is_dragging;
extern int is_rotating;

extern float cam_rot_x;
extern float cam_rot_y;
extern float current_min_val;
extern float current_max_val;

typedef enum {
    MODE_DEFORM,
    MODE_PAINT_HEAT
} Interaction_Mode;

typedef enum {
    VIEW_STRAIN,
    VIEW_STRESS,
    VIEW_ENERGY,
    VIEW_TEMPERATURE,
    VIEW_ENTROPY,
    // Reminder: Add after this and BEFORE VIEW_INVALID, it has to be the last one :)
    VIEW_INVALID
} View_Mode;

extern View_Mode view_mode;
extern Interaction_Mode interaction_mode;

#endif // COMMON_H