all: atari linux

atari: 
	m68k-atari-mint-gcc -O1 lz4.cpp do_compression.cpp -I. -o LZ4.TTP
	m68k-atari-mint-gcc -O1 -S -fverbose-asm -I. do_compression.cpp 
	m68k-atari-mint-strip LZ4.TTP

linux:
	gcc -ggdb lz4.cpp do_compression.cpp -I. -o lz4

clean:
	rm -f do_compression.s LZ4X.LZ4 LZ4X.TTP lz4
