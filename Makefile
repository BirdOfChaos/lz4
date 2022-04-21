ATARI_CC = m68k-atari-mint-gcc
ATARI_CFLAGS = -std=c89 -pedantic -O3 -I.

SRC = src/lz4.c src/lz4_compress.c src/lz4_decompress.c
INC = src/lz4.h

CFLAGS = -std=c89 -pedantic -O3 -I.

default: lz4

all: lz4 atari

# Atari ST target
atari: LZ4.TTP
LZ4.TTP: $(SRC) $(INC)
	$(ATARI_CC) $(ATARI_CFLAGS) $(SRC) -o $@
	$(ATARI_CC) $(ATARI_CFLAGS) -S -fverbose-asm lz4_compress.c
	m68k-atari-mint-strip $@

# Linux and regular POSIX target
linux: lz4
lz4: $(SRC) $(INC)
	$(CC) $(CFLAGS) $(SRC) -o $@

# Test that file compression/decompression is consistent
test: clean linux
	@cp src/lz4.c lz4-test
	@./lz4 lz4-test lz4-test.lz4
	@rm -f lz4-test
	@./lz4 -d lz4-test.lz4
	@if diff -q lz4-test src/lz4.c; \
	then \
		echo Test OK; \
	else \
		echo Test error: compression/decompression information loss; \
	fi
	@rm -f lz4-test*

clean:
	@rm -f lz4_comp.s LZ4.TTP lz4
	@rm -f arch/atari_st/LZ4.TTP
	@rm -f lz4-test lz4-test.lz4
