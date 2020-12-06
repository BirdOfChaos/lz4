all: linux

atari: 
	m68k-atari-mint-gcc -O3 lz4.c do_compression.c -I. -o LZ4.TTP
	m68k-atari-mint-gcc -O3 -S -fverbose-asm -I. do_compression.c 
	m68k-atari-mint-strip LZ4.TTP

linux:
	gcc -ggdb -std=c90 lz4.c do_compression.c -I. -o lz4

clean:
	rm -f do_compression.s LZ4X.LZ4 LZ4X.TTP lz4
