#include "test.h"

START_TEST(test_center) {
  Vertex vertices[3] = {{0, 0, 0}, {2, 2, 2}, {4, 4, 4}};
  float cx, cy, cz;
  center(vertices, 3, &cx, &cy, &cz);
  ck_assert_float_eq_tol(cx, 2.0, 1e-6);
  ck_assert_float_eq_tol(cy, 2.0, 1e-6);
  ck_assert_float_eq_tol(cz, 2.0, 1e-6);
}
END_TEST

// Создание тестов для функции translate
START_TEST(test_translate) {
  Vertex vertices[1] = {{1, 1, 1}};
  translate(vertices, 1, 1.0, 1.0, 1.0);
  ck_assert_float_eq(vertices[0].x, 2.0);
  ck_assert_float_eq(vertices[0].y, 2.0);
  ck_assert_float_eq(vertices[0].z, 2.0);
}
END_TEST

// Создание тестов для функции rotate_x
START_TEST(test_rotate_x) {
  Vertex v = {0, 1, 0};
  rotate_x(&v, 90);
  ck_assert_float_eq_tol(v.y, 0.0, 1e-6);
  ck_assert_float_eq_tol(v.z, 1.0, 1e-6);
}
END_TEST

// Создание тестов для функции rotate_y
START_TEST(test_rotate_y) {
  Vertex v = {1, 0, 0};
  rotate_y(&v, 90);
  ck_assert_float_eq_tol(v.x, 0.0, 1e-6);
  ck_assert_float_eq_tol(v.z, -1.0, 1e-6);
}
END_TEST

// Создание тестов для функции rotate_z
START_TEST(test_rotate_z) {
  Vertex v = {1, 0, 0};
  rotate_z(&v, 90);
  ck_assert_float_eq_tol(v.x, 0.0, 1e-6);
  ck_assert_float_eq_tol(v.y, 1.0, 1e-6);
}
END_TEST

// Создание тестов для функции rotate
START_TEST(test_rotate) {
  Vertex vertices[1] = {{1, 0, 0}};
  rotate(vertices, 1, 0, 90, 0);
  ck_assert_float_eq_tol(vertices[0].x, 1, 1e-6);
  ck_assert_float_eq_tol(vertices[0].z, 0, 1e-6);
}
END_TEST

Suite *rotate_suite(void) {
  Suite *s;
  TCase *tc_core;

  s = suite_create("Rotate");

  /* Core test case */
  tc_core = tcase_create("Core");

  tcase_add_test(tc_core, test_center);
  tcase_add_test(tc_core, test_translate);
  tcase_add_test(tc_core, test_rotate_x);
  tcase_add_test(tc_core, test_rotate_y);
  tcase_add_test(tc_core, test_rotate_z);
  tcase_add_test(tc_core, test_rotate);
  suite_add_tcase(s, tc_core);

  return s;
}
