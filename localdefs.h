#ifndef mc68000
#  define LZ4_MAGIC 0x184C2102  // Little endian
#else
#  define LZ4_MAGIC 0x02214C18  // Big endian
#endif

#define BLOCK_SIZE (1024*8) // 8K
#define PADDING_LITERALS 5

#define WINDOW_BITS 16
#define WINDOW_SIZE (1<<WINDOW_BITS)
#define WINDOW_MASK (WINDOW_SIZE-1)

#define MIN_MATCH 4

#define EXCESS (16+(BLOCK_SIZE/255))

#define MIN(a, b) (((a)<(b))?(a):(b))
#define MAX(a, b) (((a)>(b))?(a):(b))

#define LOAD_16(p) (*reinterpret_cast<const U16*>(&g_buf[p]))
#define LOAD_32(p) (*reinterpret_cast<const U32*>(&g_buf[p]))
#define STORE_16(p, x) (*reinterpret_cast<U16*>(&g_buf[p])=(x))
#define COPY_32(d, s) (*reinterpret_cast<U32*>(&g_buf[d])=LOAD_32(s))
#define SWAP16(i) ((i>>8)|(i<<8))
#define SWAP32(i) ( (((i) >> 24) & 0x000000FF) | (((i) >>  8) & 0x0000FF00) | \
	                  (((i) <<  8) & 0x00FF0000) | (((i) << 24) & 0xFF000000) )

#define HASH_BITS 18
#define HASH_SIZE (1<<HASH_BITS)
#define NIL (-1)

#define HASH_32(p) ((LOAD_32(p)*0x9E3779B9)>>(32-HASH_BITS))

typedef unsigned char U8;
typedef unsigned short U16;
typedef unsigned int U32;

int do_compression(U8 *, const int, int);
