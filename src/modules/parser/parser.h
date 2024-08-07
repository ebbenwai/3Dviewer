#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../model/model.h"

void parser(const char *filename, Vertex **vertices, int *vertex_count,
            Face **faces, int *face_count);
void add_vertex(Vertex **vertices, int *vertex_count, int *vertex_capacity,
                Vertex v);
void add_face(Face **faces, int *face_count, int *face_capacity, Face f);
