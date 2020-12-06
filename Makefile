all: linux

atari: 
	m68k-atari-mint-gcc -std=c89 -pedantic -O3 lz4.c lz4_comp.c -I. -o LZ4.TTP
	m68k-atari-mint-gcc -std=c89 -pedantic -O3 -S -fverbose-asm -I. lz4_comp.c 
	m68k-atari-mint-strip LZ4.TTP

linux:
	gcc -ggdb -std=c89 -pedantic lz4.c lz4_comp.c -I. -o lz4

clean:
	rm -f lz4_comp.s LZ4.TTP lz4
