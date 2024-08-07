#include "parser.h"

void add_vertex(Vertex **vertices, int *vertex_count, int *vertex_capacity,
                Vertex v) {
  if (*vertex_count >= *vertex_capacity) {
    *vertex_capacity *= 2;
    *vertices = realloc(*vertices, *vertex_capacity * sizeof(Vertex));
  }
  (*vertices)[(*vertex_count)++] = v;
}

void add_face(Face **faces, int *face_count, int *face_capacity, Face f) {
  if (*face_count >= *face_capacity) {
    *face_capacity *= 2;
    *faces = realloc(*faces, *face_capacity * sizeof(Face));
  }
  (*faces)[(*face_count)++] = f;
}

void parser(const char *filename, Vertex **vertices, int *vertex_count,
            Face **faces, int *face_count) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    return;
  }

  int vertex_capacity = 10;
  int face_capacity = 10;
  *vertices = calloc(vertex_capacity, sizeof(Vertex));
  *faces = calloc(face_capacity, sizeof(Face));
  if (!*vertices || !*faces) {
    fclose(file);
    return;
  }

  *vertex_count = 0;
  *face_count = 0;
  char line[MAX_LINE_LENGTH];

  while (fgets(line, sizeof(line), file)) {
    if (line[0] == 'v' && line[1] == ' ') {
      Vertex v = {0};
      sscanf(line, "v %f %f %f", &v.x, &v.y, &v.z);
      add_vertex(vertices, vertex_count, &vertex_capacity, v);
    } else if (line[0] == 'f' && line[1] == ' ') {
      Face f = {0};
      f.v = calloc(4, sizeof(int));
      sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &f.v[0],
             &f.v[1], &f.v[2], &f.v[3]);
      f.vertex_count = 4;
      add_face(faces, face_count, &face_capacity, f);
    }
  }

  fclose(file);
}
