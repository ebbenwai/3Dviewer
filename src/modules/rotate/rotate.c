#include "rotate.h"

void center(Vertex *vertices, int vertex_count, float *cx, float *cy,
            float *cz) {
  *cx = 0.0f;
  *cy = 0.0f;
  *cz = 0.0f;
  for (int i = 0; i < vertex_count; i++) {
    *cx += vertices[i].x;
    *cy += vertices[i].y;
    *cz += vertices[i].z;
  }
  *cx /= vertex_count;
  *cy /= vertex_count;
  *cz /= vertex_count;
}

void translate(Vertex *vertices, int vertex_count, float dx, float dy,
               float dz) {
  for (int i = 0; i < vertex_count; i++) {
    vertices[i].x += dx;
    vertices[i].y += dy;
    vertices[i].z += dz;
  }
}

void rotate_x(Vertex *v, float angle) {
  float rad = angle * M_PI / 180.0f;
  float cosA = cos(rad);
  float sinA = sin(rad);
  float y = v->y * cosA - v->z * sinA;
  float z = v->y * sinA + v->z * cosA;
  v->y = y;
  v->z = z;
}

void rotate_y(Vertex *v, float angle) {
  float rad = angle * M_PI / 180.0f;
  float cosA = cos(rad);
  float sinA = sin(rad);
  float x = v->x * cosA + v->z * sinA;
  float z = -v->x * sinA + v->z * cosA;
  v->x = x;
  v->z = z;
}

void rotate_z(Vertex *v, float angle) {
  float rad = angle * M_PI / 180.0f;
  float cosA = cos(rad);
  float sinA = sin(rad);
  float x = v->x * cosA - v->y * sinA;
  float y = v->x * sinA + v->y * cosA;
  v->x = x;
  v->y = y;
}

void rotate(Vertex *vertices, int vertex_count, float angle_x, float angle_y,
            float angle_z) {
  float cx, cy, cz;
  center(vertices, vertex_count, &cx, &cy, &cz);
  translate(vertices, vertex_count, -cx, -cy, -cz);

  for (int i = 0; i < vertex_count; i++) {
    rotate_x(&vertices[i], angle_x);
    rotate_y(&vertices[i], angle_y);
    rotate_z(&vertices[i], angle_z);
  }
  translate(vertices, vertex_count, cx, cy, cz);
}
