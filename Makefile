all: fnmatch

fnmatch: fnmatch.o

test:
	test `ls -1 | ./fnmatch '*.c' | wc -l ` = 1
	test `ls -1 | ./fnmatch 'Make*' | wc -l ` = 1
	test `ls -1 | ./fnmatch '*.c' '*.md' | wc -l ` = 2

.PHONY: all test
