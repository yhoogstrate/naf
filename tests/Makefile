
.PHONY: default all large clean

export TMPDIR=temp

zstd-seekable-test-01:
	gcc -std=gnu99 -Wall -Wextra -I../ennaf/src -I../zstd/lib -lzstd -o zstd-seekable-test-01 zstd-seekable/test_empty_frame.c  ../zstd/lib/libzstd.a

default: all

all: zstd-seekable-test-01
	@diff -q Makefile Makefile
	@mkdir -p temp
	@./test-runner.pl interface alphabet charcount small
	@echo "Success!"

large:
	@diff -q Makefile Makefile
	@mkdir -p temp
	@./test-runner.pl large
	@echo "Success!"

clean:
	@rm -f */*.out
	@rm -f */*.err
	@echo "Test directories cleaned."
