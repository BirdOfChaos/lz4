all: linux

atari: 
	m68k-atari-mint-gcc -O3 lz4.c lz4_compress.c -I. -o LZ4.TTP
	m68k-atari-mint-gcc -O3 -S -fverbose-asm -I. lz4_compress.c 
	m68k-atari-mint-strip LZ4.TTP

linux:
	gcc -ggdb -std=c90 lz4.c lz4_compress.c -I. -o lz4

clean:
	rm -f lz4_compress.s LZ4.TTP lz4
