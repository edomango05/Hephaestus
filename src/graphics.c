#include "common.h"
#include "graphics.h"

static void get_color(float val, float* r, float* g, float* b) {
    float norm = (val - current_min_val) / (current_max_val - current_min_val + 1e-6f);
    if (norm < 0.0f) norm = 0.0f;
    if (norm > 1.0f) norm = 1.0f;

    if (norm < 0.5f) {
        *r = 0.0f;
        *g = norm * 2.0f;
        *b = 1.0f - norm * 2.0f;
    } else {
        *r = (norm - 0.5f) * 2.0f;
        *g = 1.0f - (norm - 0.5f) * 2.0f;
        *b = 0.0f;
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 5.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0);

    glRotatef(cam_rot_x, 1.0f, 0.0f, 0.0f);
    glRotatef(cam_rot_y, 0.0f, 1.0f, 0.0f);

    glPointSize(6.0f);
    glBegin(GL_POINTS);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                Point* p = &grid[i][j][k];
                float val = (view_mode == 0) ? p->strain_vm : ((view_mode == 1) ? p->stress_vm : ((view_mode == 2) ? p->energy : p->delta_T));
                
                float r, g, b;
                get_color(val, &r, &g, &b);
                glColor3f(r, g, b);

                if (i == selected_i && j == selected_j && k == selected_k && is_dragging) glColor3f(1, 1, 1);
                
                glVertex3f(p->x, p->y, p->z);
            }
        }
    }
    glEnd();
    
    glColor3f(1, 1, 1);
    glMatrixMode(GL_PROJECTION);
    glPushMatrix(); glLoadIdentity(); gluOrtho2D(0, 800, 0, 600);
    glMatrixMode(GL_MODELVIEW); glPushMatrix(); glLoadIdentity();
    
    char* mode_str = (view_mode == 0) ? "Strain" : ((view_mode == 1) ? "Stress" : ((view_mode == 2) ? "Energy" : "Temperature"));
    char* interact_str = (interaction_mode == 0) ? "DEFORM" : "PAINT HEAT";
    
    char info1[200], info2[200];
    sprintf(info1, "View: [%s] | Action: [%s]", mode_str, interact_str);
    sprintf(info2, "E: %.1f (e/E) | Nu: %.2f (p/P) | M: Toggle Action | SPACE: Toggle View | R: Reset", E, nu);
    
    glRasterPos2i(10, 570);
    for (int i = 0; info1[i] != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info1[i]);
    glRasterPos2i(10, 545);
    for (int i = 0; info2[i] != '\0'; i++) glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, info2[i]);

    glPopMatrix(); glMatrixMode(GL_PROJECTION); glPopMatrix(); glMatrixMode(GL_MODELVIEW);
    glutSwapBuffers();
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}