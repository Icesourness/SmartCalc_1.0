CFLAGS = -std=c11 -Wall -Wextra -Werror
FILES = gtk.c functions.c s21_smartcalc.c
TESTFILES = tests.c

OS=$(shell uname)

ifeq (${OS}, Darwin)
	LIBS = -L. -lcheck -lm
else
	LIBS = -L. -lcheck -lm -lsubunit
endif 

all: $(FILES)
	gcc $(CFLAGS) -rdynamic $(FILES) -o smartcalc $(LIBS) `pkg-config --cflags --libs gtk+-3.0`

clean:
	rm -f smartcalc

check:
	clang-format -style=Google -n $(FILES) *.h
	
clang:
	clang-format -style=Google -i $(FILES) *.h