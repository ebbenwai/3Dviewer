#include "test.h"

START_TEST(test_add_vertex) {
  int vertex_count = 0;
  int vertex_capacity = 2;
  Vertex *vertices = malloc(vertex_capacity * sizeof(Vertex));
  ck_assert_ptr_ne(vertices, NULL);

  Vertex v1 = {1.0, 2.0, 3.0};
  Vertex v2 = {4.0, 5.0, 6.0};
  Vertex v3 = {7.0, 8.0, 9.0};

  add_vertex(&vertices, &vertex_count, &vertex_capacity, v1);
  ck_assert_int_eq(vertex_count, 1);
  ck_assert_float_eq(vertices[0].x, 1.0);
  ck_assert_float_eq(vertices[0].y, 2.0);
  ck_assert_float_eq(vertices[0].z, 3.0);

  add_vertex(&vertices, &vertex_count, &vertex_capacity, v2);
  ck_assert_int_eq(vertex_count, 2);
  ck_assert_float_eq(vertices[1].x, 4.0);

  add_vertex(&vertices, &vertex_count, &vertex_capacity, v3);
  ck_assert_int_eq(vertex_count, 3);
  ck_assert_int_eq(vertex_capacity, 4);

  free(vertices);
}
END_TEST

START_TEST(test_add_face) {
  int face_count = 0;
  int face_capacity = 2;
  Face *faces = malloc(face_capacity * sizeof(Face));
  ck_assert_ptr_ne(faces, NULL);

  Face f1 = {malloc(4 * sizeof(int)), 4};
  f1.v[0] = 1;
  f1.v[1] = 2;
  f1.v[2] = 3;
  f1.v[3] = 4;

  Face f2 = {malloc(4 * sizeof(int)), 4};
  f2.v[0] = 5;
  f2.v[1] = 6;
  f2.v[2] = 7;
  f2.v[3] = 8;

  Face f3 = {malloc(4 * sizeof(int)), 4};
  f3.v[0] = 9;
  f3.v[1] = 10;
  f3.v[2] = 11;
  f3.v[3] = 12;

  add_face(&faces, &face_count, &face_capacity, f1);
  ck_assert_int_eq(face_count, 1);
  ck_assert_int_eq(faces[0].v[0], 1);

  add_face(&faces, &face_count, &face_capacity, f2);
  ck_assert_int_eq(face_count, 2);
  ck_assert_int_eq(faces[1].v[0], 5);

  add_face(&faces, &face_count, &face_capacity, f3);
  ck_assert_int_eq(face_count, 3);
  ck_assert_int_eq(face_capacity, 4);

  for (int i = 0; i < 3; i++) {
    free(faces[i].v);
  }
  free(faces);
}
END_TEST

START_TEST(test_parser) {
  FILE *file = fopen("test.obj", "w");
  ck_assert_ptr_ne(file, NULL);
  fprintf(file, "v 1.0 2.0 3.0\n");
  fprintf(file, "v 4.0 5.0 6.0\n");
  fprintf(file, "f 1/1/1 2/2/2 3/3/3 4/4/4\n");
  fclose(file);

  Vertex *vertices = NULL;
  Face *faces = NULL;
  int vertex_count = 0;
  int face_count = 0;

  parser("test.obj", &vertices, &vertex_count, &faces, &face_count);

  ck_assert_int_eq(vertex_count, 2);
  ck_assert_float_eq(vertices[0].x, 1.0);
  ck_assert_float_eq(vertices[1].x, 4.0);

  ck_assert_int_eq(face_count, 1);
  ck_assert_int_eq(faces[0].v[0], 1);

  free(vertices);
  for (int i = 0; i < face_count; i++) {
    free(faces[i].v);
  }
  free(faces);

  remove("test.obj");
}
END_TEST

Suite *parser_suite(void) {
  Suite *s = suite_create("Parser");

  TCase *tc_core = tcase_create("Core");
  tcase_add_test(tc_core, test_add_vertex);
  tcase_add_test(tc_core, test_add_face);
  tcase_add_test(tc_core, test_parser);
  suite_add_tcase(s, tc_core);

  return s;
}
