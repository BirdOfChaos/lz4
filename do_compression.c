#include <localdefs.h>

int do_compression(U8 *g_buf, const int max_chain, int n)
{

  int i;

  static int head[HASH_SIZE];
  static int tail[WINDOW_SIZE];

  for (i=0; i<HASH_SIZE; ++i)
      head[i]=NIL;

  int op=BLOCK_SIZE;
  int pp=0;

  int p=0;
  while (p<n)
  {
    int best_len=0;
    int dist=0;

    const int max_match=(n-PADDING_LITERALS)-p;
    if (max_match>=MAX(12-PADDING_LITERALS, MIN_MATCH))
    {
      const int limit=MAX(p-WINDOW_SIZE, NIL);
      int chain_len=max_chain;

      int s=head[HASH_32(p)];
      while (s>limit)
      {
        if (g_buf[s+best_len]==g_buf[p+best_len] && LOAD_32(s)==LOAD_32(p))
        {
          int len=MIN_MATCH;
          while (len<max_match && g_buf[s+len]==g_buf[p+len])
            ++len;

          if (len>best_len)
          {
            best_len=len;
            dist=p-s;

            if (len==max_match)
              break;
          }
        }

        if (--chain_len==0)
          break;

        s=tail[s&WINDOW_MASK];
      }
    }

    if (best_len>=MIN_MATCH)
    {
      int len=best_len-MIN_MATCH;
      const int nib=MIN(len, 15);

      if (pp!=p)
      {
        const int run=p-pp;
        if (run>=15)
        {
          g_buf[op++]=(15<<4)+nib;

          int j=run-15;
          for (; j>=255; j-=255)
            g_buf[op++]=255;
          g_buf[op++]=j;
        }
        else
          g_buf[op++]=(run<<4)+nib;

        /* wild_copy(op, pp, run); */
        COPY_32(op, pp);
        COPY_32(op+4, pp+4);
        for (i=8; i<run; i+=8)
        {
            COPY_32(op+i, pp+i);
            COPY_32(op+4+i, pp+4+i);
        }
        op+=run;
      }
      else
        g_buf[op++]=nib;

#ifdef LZ4_LITTLE
      /* Little endian */
      STORE_16(op, dist);
#else
      /* Big endian */
      STORE_16(op, SWAP16(dist));
#endif
      op+=2;

      if (len>=15)
      {
        len-=15;
        for (; len>=255; len-=255)
          g_buf[op++]=255;
        g_buf[op++]=len;
      }

      pp=p+best_len;

      while (p<pp)
      {
        const U32 h=HASH_32(p);
        tail[p&WINDOW_MASK]=head[h];
        head[h]=p++;
      }
    }
    else
    {
      const U32 h=HASH_32(p);
      tail[p&WINDOW_MASK]=head[h];
      head[h]=p++;
    }
  }

  if (pp!=p)
  {
    const int run=p-pp;
    if (run>=15)
    {
      g_buf[op++]=15<<4;

      int j=run-15;
      for (; j>=255; j-=255)
        g_buf[op++]=255;
      g_buf[op++]=j;
    }
    else
      g_buf[op++]=run<<4;

    /* wild_copy(op, pp, run); */
    COPY_32(op, pp);
    COPY_32(op+4, pp+4);
    for (i=8; i<run; i+=8)
      {
          COPY_32(op+i, pp+i);
          COPY_32(op+4+i, pp+4+i);
      }
    op+=run;
  }
    
  return op-BLOCK_SIZE;
}