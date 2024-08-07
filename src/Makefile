GCC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11 -pedantic
TSRC = $(wildcard test/*.c)
SRC = $(wildcard modules/scale/*.c modules/rotate/*.c modules/move/*.c modules/parser/*.c modules/model/*.c)
QTSRC = $(wildcard qt/3dviewer/*.cc)
TOBJ = $(TSRC:.c=.o)
OBJ = $(SRC:.c=.o)
UNAME = $(shell uname -s)
TNAME = 3dviewer_test

ifeq ($(UNAME), Linux)
	LIBS = -lcheck -lm -lsubunit
endif
ifeq ($(UNAME), Darwin)
	LIBS = -lcheck
endif

all: clean dvi tests check_style install gcov_report

tests:
	$(GCC) $(TSRC) $(SRC) $(CFLAGS) $(LIBS) -o $(TNAME)
	./$(TNAME)

install:
	cd qt/3dviewer && qmake6 && make && mv 3dviewer ../../

uninstall:
	cd qt/3dviewer && make distclean
	rm 3dviewer

dvi:
	open doc.md

check_style:
	clang-format -n --style="{BasedOnStyle: Google}" --verbose $(SRC) $(QTSRC) $(TSRC) $(SRC:.c=.h) $(filter-out qt/3dviewer/main.h, $(QTSRC:.cc=.h))

fix_style:
	clang-format -i --style="{BasedOnStyle: Google}" --verbose $(SRC) $(QTSRC) $(TSRC) $(SRC:.c=.h) $(filter-out qt/3dviewer/main.h, $(QTSRC:.cc=.h))

gcov_report:
	$(GCC) --coverage $(TSRC) $(SRC) $(CFLAGS) $(LIBS) -o $(TNAME)
	./$(TNAME)
	lcov --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory report
	open report/index.html

clean:
	rm -rf $(TOBJ) *.gcno *.gcda* $(TNAME) coverage.info report
