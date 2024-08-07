#include "scale.h"

void scale_vertices(Vertex *vertices, int num_vertices, float scale) {
  for (int i = 0; i < num_vertices; ++i) {
    vertices[i].x *= scale;
    vertices[i].y *= scale;
    vertices[i].z *= scale;
  }
}
