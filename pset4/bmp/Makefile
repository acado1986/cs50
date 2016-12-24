copy : copy.c
	clang -ggdb3 -std=c11 copy.c -o copy
whodunit : whodunit.c
	clang -ggdb3 -std=c11 whodunit.c -o whodunit
resize : resize.c bmp.h
	clang -ggdb3 -std=c11 resize.c -o resize
clean:
	rm -f *.o a.out *.txt core copy whodunit count resize
