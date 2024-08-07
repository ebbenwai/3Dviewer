#include "test.h"

START_TEST(test_scale_positive_factor) {
  Vertex vertices[3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  int num_vertices = 3;
  float scale = 2.0;

  scale_vertices(vertices, num_vertices, scale);

  ck_assert_float_eq(vertices[0].x, 2.0);
  ck_assert_float_eq(vertices[0].y, 4.0);
  ck_assert_float_eq(vertices[0].z, 6.0);

  ck_assert_float_eq(vertices[1].x, 8.0);
  ck_assert_float_eq(vertices[1].y, 10.0);
  ck_assert_float_eq(vertices[1].z, 12.0);

  ck_assert_float_eq(vertices[2].x, 14.0);
  ck_assert_float_eq(vertices[2].y, 16.0);
  ck_assert_float_eq(vertices[2].z, 18.0);
}
END_TEST

START_TEST(test_scale_negative_factor) {
  Vertex vertices[3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  int num_vertices = 3;
  float scale = -1.0;

  scale_vertices(vertices, num_vertices, scale);

  ck_assert_float_eq(vertices[0].x, -1.0);
  ck_assert_float_eq(vertices[0].y, -2.0);
  ck_assert_float_eq(vertices[0].z, -3.0);

  ck_assert_float_eq(vertices[1].x, -4.0);
  ck_assert_float_eq(vertices[1].y, -5.0);
  ck_assert_float_eq(vertices[1].z, -6.0);

  ck_assert_float_eq(vertices[2].x, -7.0);
  ck_assert_float_eq(vertices[2].y, -8.0);
  ck_assert_float_eq(vertices[2].z, -9.0);
}
END_TEST

START_TEST(test_scale_zero_factor) {
  Vertex vertices[3] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
  int num_vertices = 3;
  float scale = 0.0;

  scale_vertices(vertices, num_vertices, scale);

  ck_assert_float_eq(vertices[0].x, 0.0);
  ck_assert_float_eq(vertices[0].y, 0.0);
  ck_assert_float_eq(vertices[0].z, 0.0);

  ck_assert_float_eq(vertices[1].x, 0.0);
  ck_assert_float_eq(vertices[1].y, 0.0);
  ck_assert_float_eq(vertices[1].z, 0.0);

  ck_assert_float_eq(vertices[2].x, 0.0);
  ck_assert_float_eq(vertices[2].y, 0.0);
  ck_assert_float_eq(vertices[2].z, 0.0);
}
END_TEST

Suite *scale_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Scale");

  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_scale_positive_factor);
  tcase_add_test(tc_core, test_scale_negative_factor);
  tcase_add_test(tc_core, test_scale_zero_factor);
  suite_add_tcase(s, tc_core);

  return s;
}
