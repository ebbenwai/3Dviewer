#include "test.h"

int main() {
  int number_failed = 0;
  SRunner *sr = srunner_create(NULL);

  srunner_add_suite(sr, move_suite());
  srunner_add_suite(sr, scale_suite());
  srunner_add_suite(sr, parser_suite());
  srunner_add_suite(sr, rotate_suite());

  srunner_run_all(sr, CK_NORMAL);
  number_failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return !number_failed ? 0 : 1;
}
