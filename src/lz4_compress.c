#include <src/lz4.h>
int lz4_compress(U8 *g_buf, const int max_chain, const int uc_length)
{

  int i;
  
  int op=BLOCK_SIZE;
  int pp=0;
  int p=0;
  
  int s, dist, limit;
  int len, chain_len, best_len;
  int max_match, nib;
  int run, j;

  static int head[HASH_SIZE];
  static int tail[WINDOW_SIZE];

  for (i=0; i<HASH_SIZE; ++i)
      head[i]=NIL;

  while (p<uc_length)
  {
    best_len=0;
    dist=0;

    max_match=(uc_length-PADDING_LITERALS)-p;
    if (max_match>=MAX(12-PADDING_LITERALS, MIN_MATCH))
    {
      limit=MAX(p-WINDOW_SIZE, NIL);
      chain_len=max_chain;

      s=head[HASH_32(p)];
      while (s>limit)
      {
        if (g_buf[s+best_len]==g_buf[p+best_len] && LOAD_32(s)==LOAD_32(p))
        {
          len=MIN_MATCH;
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
      len=best_len-MIN_MATCH;
      nib=MIN(len, 15);

      if (pp!=p)
      {
        run=p-pp;
        if (run>=15)
        {
          g_buf[op++]=(15<<4)+nib;

          j=run-15;
          for (; j>=255; j-=255)
            g_buf[op++]=255;
          g_buf[op++]=j;
        }
        else
          g_buf[op++]=(run<<4)+nib;

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
    run=p-pp;
    if (run>=15)
    {
      g_buf[op++]=15<<4;

      j=run-15;
      for (; j>=255; j-=255)
        g_buf[op++]=255;
      g_buf[op++]=j;
    }
    else
      g_buf[op++]=run<<4;

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
