#pragma once

#include <math.h>

#include "../model/model.h"

#ifndef M_PI
#define M_PI (3.14159265358979323846)
#endif

void rotate_x(Vertex *v, float angle);
void rotate_y(Vertex *v, float angle);
void rotate_z(Vertex *v, float angle);
void rotate(Vertex *vertices, int vertex_count, float angleX, float angleY,
            float angleZ);
void center(Vertex *vertices, int vertex_count, float *cx, float *cy,
            float *cz);
void translate(Vertex *vertices, int vertex_count, float dx, float dy,
               float dz);
