all: threaded naive queue_test

threaded:
	clang -o threaded threaded.c

naive:
	clang -o naive naive.c

queue_test:
	clang -o queue_test queue_test.c

.PHONY: clean
clean:
	rm -f queue_test naive threaded
