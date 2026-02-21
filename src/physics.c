#include "common.h"
#include "physics.h"

void init_material() {
    if (nu >= 0.49f) nu = 0.49f;
    if (nu <= -0.99f) nu = -0.99f;
    if (E < 10.0f) E = 10.0f;

    mu_lame = E / (2.0f * (1.0f + nu));
    lambda_lame = (E * nu) / ((1.0f + nu) * (1.0f - 2.0f * nu));
}

void init_grid() {
    delta_L = SIZE / (N - 1);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                float px = -SIZE/2.0f + i * delta_L;
                float py = -SIZE/2.0f + j * delta_L;
                float pz = -SIZE/2.0f + k * delta_L;
                
                grid[i][j][k].X = px; grid[i][j][k].Y = py; grid[i][j][k].Z = pz;
                grid[i][j][k].x = px; grid[i][j][k].y = py; grid[i][j][k].z = pz;
                grid[i][j][k].u = 0;  grid[i][j][k].v = 0;  grid[i][j][k].w = 0;
                grid[i][j][k].delta_T = 0.0f;
            }
        }
    }
}

static float get_deriv(int i, int j, int k, int axis, int var) {
    int i_next = i, j_next = j, k_next = k;
    int i_prev = i, j_prev = j, k_prev = k;
    float dL = delta_L;

    if (axis == 0) { 
        if (i == 0) { i_next = 1; i_prev = 0; }
        else if (i == N - 1) { i_next = N - 1; i_prev = N - 2; }
        else { i_next = i + 1; i_prev = i - 1; dL *= 2.0f; }
    } else if (axis == 1) { 
        if (j == 0) { j_next = 1; j_prev = 0; }
        else if (j == N - 1) { j_next = N - 1; j_prev = N - 2; }
        else { j_next = j + 1; j_prev = j - 1; dL *= 2.0f; }
    } else if (axis == 2) { 
        if (k == 0) { k_next = 1; k_prev = 0; }
        else if (k == N - 1) { k_next = N - 1; k_prev = N - 2; }
        else { k_next = k + 1; k_prev = k - 1; dL *= 2.0f; }
    }

    float val_next, val_prev;
    if (var == 0) { val_next = grid[i_next][j_next][k_next].u; val_prev = grid[i_prev][j_prev][k_prev].u; } 
    else if (var == 1) { val_next = grid[i_next][j_next][k_next].v; val_prev = grid[i_prev][j_prev][k_prev].v; } 
    else { val_next = grid[i_next][j_next][k_next].w; val_prev = grid[i_prev][j_prev][k_prev].w; }

    return (val_next - val_prev) / dL;
}

void compute_mechanics() {
    current_max_val = 0.0001f;
    current_min_val = 1e9f;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                grid[i][j][k].u = grid[i][j][k].x - grid[i][j][k].X;
                grid[i][j][k].v = grid[i][j][k].y - grid[i][j][k].Y;
                grid[i][j][k].w = grid[i][j][k].z - grid[i][j][k].Z;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < N; k++) {
                Point* p = &grid[i][j][k];
                
                float e_xx = get_deriv(i, j, k, 0, 0);
                float e_yy = get_deriv(i, j, k, 1, 1);
                float e_zz = get_deriv(i, j, k, 2, 2);
                float e_xy = 0.5f * (get_deriv(i, j, k, 1, 0) + get_deriv(i, j, k, 0, 1));
                float e_yz = 0.5f * (get_deriv(i, j, k, 2, 1) + get_deriv(i, j, k, 1, 2));
                float e_xz = 0.5f * (get_deriv(i, j, k, 2, 0) + get_deriv(i, j, k, 0, 2));

                float e_th = alpha * p->delta_T;
                float tr_e = e_xx + e_yy + e_zz;

                float s_xx = lambda_lame * (tr_e - 3.0f * e_th) + 2.0f * mu_lame * (e_xx - e_th);
                float s_yy = lambda_lame * (tr_e - 3.0f * e_th) + 2.0f * mu_lame * (e_yy - e_th);
                float s_zz = lambda_lame * (tr_e - 3.0f * e_th) + 2.0f * mu_lame * (e_zz - e_th);
                float s_xy = 2.0f * mu_lame * e_xy;
                float s_yz = 2.0f * mu_lame * e_yz;
                float s_xz = 2.0f * mu_lame * e_xz;

                p->strain_vm = sqrt(e_xx*e_xx + e_yy*e_yy + e_zz*e_zz + 2*(e_xy*e_xy + e_yz*e_yz + e_xz*e_xz));
                p->stress_vm = sqrt(0.5f * ((s_xx-s_yy)*(s_xx-s_yy) + (s_yy-s_zz)*(s_yy-s_zz) + (s_zz-s_xx)*(s_zz-s_xx) + 6.0f*(s_xy*s_xy + s_yz*s_yz + s_xz*s_xz)));
                p->energy = 0.5f * (s_xx*(e_xx-e_th) + s_yy*(e_yy-e_th) + s_zz*(e_zz-e_th) + 2.0f*(s_xy*e_xy + s_yz*e_yz + s_xz*e_xz));

                float val = (view_mode == 0) ? p->strain_vm : ((view_mode == 1) ? p->stress_vm : ((view_mode == 2) ? p->energy : p->delta_T));
                if (val > current_max_val) current_max_val = val;
                if (val < current_min_val) current_min_val = val;
            }
        }
    }
}