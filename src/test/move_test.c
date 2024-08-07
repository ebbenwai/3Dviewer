#include "test.h"

START_TEST(test_move_positive_displacement) {
  Vertex vertices[3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  int num_vertices = 3;
  float tx = 1.0, ty = 1.0, tz = 1.0;

  move_model(vertices, num_vertices, tx, ty, tz);

  ck_assert_float_eq(vertices[0].x, 2.0);
  ck_assert_float_eq(vertices[0].y, 3.0);
  ck_assert_float_eq(vertices[0].z, 4.0);

  ck_assert_float_eq(vertices[1].x, 5.0);
  ck_assert_float_eq(vertices[1].y, 6.0);
  ck_assert_float_eq(vertices[1].z, 7.0);

  ck_assert_float_eq(vertices[2].x, 8.0);
  ck_assert_float_eq(vertices[2].y, 9.0);
  ck_assert_float_eq(vertices[2].z, 10.0);
}
END_TEST

START_TEST(test_move_negative_displacement) {
  Vertex vertices[3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  int num_vertices = 3;
  float tx = -1.0, ty = -1.0, tz = -1.0;

  move_model(vertices, num_vertices, tx, ty, tz);

  ck_assert_float_eq(vertices[0].x, 0.0);
  ck_assert_float_eq(vertices[0].y, 1.0);
  ck_assert_float_eq(vertices[0].z, 2.0);

  ck_assert_float_eq(vertices[1].x, 3.0);
  ck_assert_float_eq(vertices[1].y, 4.0);
  ck_assert_float_eq(vertices[1].z, 5.0);

  ck_assert_float_eq(vertices[2].x, 6.0);
  ck_assert_float_eq(vertices[2].y, 7.0);
  ck_assert_float_eq(vertices[2].z, 8.0);
}
END_TEST

START_TEST(test_move_zero_displacement) {
  Vertex vertices[3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  int num_vertices = 3;
  float tx = 0.0, ty = 0.0, tz = 0.0;

  move_model(vertices, num_vertices, tx, ty, tz);

  ck_assert_float_eq(vertices[0].x, 1.0);
  ck_assert_float_eq(vertices[0].y, 2.0);
  ck_assert_float_eq(vertices[0].z, 3.0);

  ck_assert_float_eq(vertices[1].x, 4.0);
  ck_assert_float_eq(vertices[1].y, 5.0);
  ck_assert_float_eq(vertices[1].z, 6.0);

  ck_assert_float_eq(vertices[2].x, 7.0);
  ck_assert_float_eq(vertices[2].y, 8.0);
  ck_assert_float_eq(vertices[2].z, 9.0);
}
END_TEST

Suite *move_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Move");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_move_positive_displacement);
  tcase_add_test(tc_core, test_move_negative_displacement);
  tcase_add_test(tc_core, test_move_zero_displacement);
  suite_add_tcase(s, tc_core);

  return s;
}
