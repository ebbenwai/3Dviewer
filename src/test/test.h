#ifndef TEST_H
#define TEST_H

#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../modules/move/move.h"
#include "../modules/scale/scale.h"
#include "../modules/parser/parser.h"
#include "../modules/rotate/rotate.h"

Suite *move_suite();
Suite *scale_suite();
Suite *parser_suite();
Suite *rotate_suite();

#endif // TEST_H
