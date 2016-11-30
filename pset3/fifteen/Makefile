#
# Makefile
#
# Computer Science 50
# Problem Set 3
#
test: test.c
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o test test.c -lcs50 -lm
fifteen: fifteen.c
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o fifteen fifteen.c -lcs50 -lm

clean:
	rm -f *.o a.out core fifteen log.txt