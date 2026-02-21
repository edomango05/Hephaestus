#include "common.h"
#include "input.h"
#include "physics.h"

void keyboard(unsigned char key, int x, int y) {
    switch(key) {
        case ' ': view_mode = (view_mode + 1) % 5; break;
        case 'm': case 'M': interaction_mode = (interaction_mode == 0) ? 1 : 0; break;
        case 'e': E *= 0.9f; init_material(); break;
        case 'E': E *= 1.1f; init_material(); break;
        case 'p': nu -= 0.05f; init_material(); break;
        case 'P': nu += 0.05f; init_material(); break;
        case 'r': case 'R': init_grid(); cam_rot_x = 30.0f; cam_rot_y = 45.0f; break;
        case 27: exit(0); break;
    }
    compute_mechanics();
    glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) { is_rotating = 1; last_mouse_x = x; last_mouse_y = y; } 
        else if (state == GLUT_UP) is_rotating = 0;
    } 
    else if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            is_dragging = 1; last_mouse_x = x; last_mouse_y = y;
            
            GLint viewport[4]; GLdouble modelview[16], projection[16], winX, winY, winZ;
            glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
            glGetDoublev(GL_PROJECTION_MATRIX, projection);
            glGetIntegerv(GL_VIEWPORT, viewport);

            float min_dist = 1e9f;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    for (int k = 0; k < N; k++) {
                        gluProject(grid[i][j][k].x, grid[i][j][k].y, grid[i][j][k].z, modelview, projection, viewport, &winX, &winY, &winZ);
                        winY = viewport[3] - winY; 
                        float dist = sqrt(pow(winX - x, 2) + pow(winY - y, 2));
                        if (dist < min_dist) { min_dist = dist; selected_i = i; selected_j = j; selected_k = k; }
                    }
                }
            }
        } else if (state == GLUT_UP) {
            is_dragging = 0; selected_i = -1; glutPostRedisplay();
        }
    }
}

void motion(int x, int y) {
    if (is_rotating) {
        cam_rot_y += (x - last_mouse_x) * 0.5f;
        cam_rot_x += (y - last_mouse_y) * 0.5f;
        last_mouse_x = x; last_mouse_y = y;
        glutPostRedisplay();
    } 
    else if (is_dragging && selected_i != -1) {
        float dx = (x - last_mouse_x) * 0.01f;
        float dy = -(y - last_mouse_y) * 0.01f; 
        Point* sp = &grid[selected_i][selected_j][selected_k];
        float radius = 1.0f; 
        
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                for (int k = 0; k < N; k++) {
                    Point* p = &grid[i][j][k];
                    float dist = sqrt(pow(p->X - sp->X, 2) + pow(p->Y - sp->Y, 2) + pow(p->Z - sp->Z, 2));
                    float weight = exp(-(dist * dist) / (radius * radius));
                    
                    if (interaction_mode == 0) {
                        p->x += dx * weight * cos(-cam_rot_y * M_PI / 180.0f);
                        p->z += dx * weight * sin(-cam_rot_y * M_PI / 180.0f);
                        p->y += dy * weight; 
                    } else {
                        p->delta_T += dy * weight * 50.0f;
                    }
                }
            }
        }
        last_mouse_x = x; last_mouse_y = y;
        compute_mechanics();
        glutPostRedisplay();
    }
}