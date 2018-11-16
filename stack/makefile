CFLAGS = -g -Wall -fprofile-arcs -ftest-coverage


all: tests
tests: tests.o stack.o
	gcc stack.o tests.o -o tests ${CFLAGS}
tests.o: tests.c
	gcc -c tests.c ${CFLAGS}
stack.o: stack.c
	gcc -c stack.c ${CFLAGS}
clean:
	rm -rf tests *.o *.gcov *.dSYM *.gcda *.gcno
