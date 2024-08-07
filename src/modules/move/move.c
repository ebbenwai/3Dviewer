#include "move.h"

void move_model(Vertex *vertices, int num_vertices, float tx, float ty,
                float tz) {
  for (int i = 0; i < num_vertices; ++i) {
    vertices[i].x += tx;
    vertices[i].y += ty;
    vertices[i].z += tz;
  }
}
