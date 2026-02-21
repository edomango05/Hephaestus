#ifndef COMMON_H
#define COMMON_H

#include <GL/glut.h>
#include <math.h>
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
} Point;

extern float E;
extern float nu;
extern float alpha;
extern float lambda_lame, mu_lame;
extern Point grid[N][N][N];
extern float delta_L;

extern int view_mode;
extern int interaction_mode;
extern int selected_i, selected_j, selected_k;
extern int last_mouse_x, last_mouse_y;
extern int is_dragging;
extern int is_rotating;

extern float cam_rot_x;
extern float cam_rot_y;
extern float current_min_val;
extern float current_max_val;

#endif // COMMON_H