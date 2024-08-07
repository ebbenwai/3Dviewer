#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 128

typedef struct {
  float x, y, z;
} Vertex;

typedef struct {
  int *v;
  int vertex_count;
} Face;
