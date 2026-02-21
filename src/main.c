#include "common.h"
#include "physics.h"
#include "graphics.h"
#include "input.h"

float E = 1000.0f;
float nu = 0.3f;
float alpha = 0.05f;
float lambda_lame, mu_lame;
Point grid[N][N][N];
float delta_L;

int view_mode = 0;
int interaction_mode = 0;
int selected_i = -1, selected_j = -1, selected_k = -1;
int last_mouse_x, last_mouse_y;
int is_dragging = 0;
int is_rotating = 0;

float cam_rot_x = 30.0f;
float cam_rot_y = 45.0f;
float current_min_val = 0.0f;
float current_max_val = 1.0f;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutCreateWindow("Thermoelasticity Simulator");

    glEnable(GL_DEPTH_TEST);
    
    init_material();
    init_grid();
    compute_mechanics();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    glutMainLoop();
    return 0;
}