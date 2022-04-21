#include <src/lz4.h>

int lz4_decompress(U8 *g_buf, const int comp_len, int *pp)
{
  int len;
  int i, p, ip, ip_end, s;
  int error, run;
 
  p=0;

  ip=BLOCK_SIZE;
  ip_end=ip+comp_len;

  for (;;)
  {
    const int token=g_buf[ip++];
    if (token>=16)
    {
      run=token>>4;
      if (run==15)
      {
        for (;;)
        {
          const int c=g_buf[ip++];
          run+=c;
          if (c!=255)
            break;
        }
      }
      if ((p+run)>BLOCK_SIZE)
        return -1;

      COPY_32(p, ip);
      COPY_32(p+4, ip+4);
      for (i=8; i<run; i+=8)
      {
          COPY_32(p+i, ip+i);
          COPY_32(p+4+i, ip+4+i);
      }
      p+=run;
      ip+=run;
      if (ip>=ip_end)
        break;
    }

    s=p-LOAD_16(ip);
    ip+=2;
    if (s<0)
      return -1;

    len=(token&15)+MIN_MATCH;
    if (len==(15+MIN_MATCH))
    {
      for (;;)
      {
        const int c=g_buf[ip++];
        len+=c;
        if (c!=255)
          break;
      }
    }
    if ((p+len)>BLOCK_SIZE)
      return -1;

    if ((p-s)>=4)
    {
      /* wild_copy(p, s, len); */
      COPY_32(p, s);
      COPY_32(p+4, s+4);
      for (i=8; i<len; i+=8)
      {
          COPY_32(p+i, s+i);
          COPY_32(p+4+i, s+4+i);
      }
      p+=len;
    }
    else
    {
      while (len--!=0)
        g_buf[p++]=g_buf[s++];
    }
  }
  *pp = p;
  return 0;
}
