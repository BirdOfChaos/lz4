ATARI_CC = m68k-atari-mint-gcc
ATARI_CFLAGS = -std=c89 -pedantic -O3 -I.

SRC = lz4.c lz4_comp.c

CFLAGS = -std=c89 -pedantic -O3 -I.

default: lz4

all: lz4 atari

# Atari ST target
atari: LZ4.TTP
LZ4.TTP: $(SRC) 
	$(ATARI_CC) $(ATARI_CFLAGS) $(SRC) -o $@
	$(ATARI_CC) $(ATARI_CFLAGS) -S -fverbose-asm lz4_comp.c
	m68k-atari-mint-strip $@

# Linux and regular POSIX target
linux: lz4
lz4:
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -f lz4_comp.s LZ4.TTP lz4
	rm -f arch/atari_st/LZ4.TTP
