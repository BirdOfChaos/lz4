/*

lz4 - utility to compress/decompress files using the LZ4 algorithm

See accompanying LICENSE file for licensing terms

Authors: Laurent Chardon, Ilya Muravyov

Original lz4x code written and placed in the public domain by Ilya Muravyov

*/
#define _FILE_OFFSET_BITS 64
#if __STDC_VERSION__ >= 200112L
#  define _ftelli64 ftello
#else
#  define _ftelli64 ftell
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "src/lz4.h"

FILE* g_in;
FILE* g_out;

U8 *g_buf=NULL;

void compress(const int max_chain)
{
  int n;
  int comp_len, clbe;

  while ((n=fread(g_buf, 1, BLOCK_SIZE, g_in))>0)
  {
    comp_len = lz4_compress(g_buf, max_chain, n);

#ifdef LZ4_LITTLE
    /* Little endian */
    fwrite(&comp_len, 1, sizeof(comp_len), g_out);
#else
    /* Big endian */
    clbe = SWAP32(comp_len);
    fwrite(&clbe, 1, sizeof(clbe), g_out);
#endif
    fwrite(&g_buf[BLOCK_SIZE], 1, comp_len, g_out);

    fprintf(stderr, "%li -> %li\n\r", _ftelli64(g_in), _ftelli64(g_out));
  }
}

int decompress()
{
  int comp_len, p;
  int error;

  while (fread(&comp_len, 1, sizeof(comp_len), g_in)>0)
  {
    if (comp_len==LZ4_MAGIC)
      continue;

    if (comp_len<2 || comp_len>(BLOCK_SIZE+EXCESS)
        || fread(&g_buf[BLOCK_SIZE], 1, comp_len, g_in)!=comp_len)
      return -1;
    
    error=lz4_decompress(g_buf, comp_len, &p);
    if (error != 0) 
    {
      perror("Error decompressing");
      exit(error);
    }

    if (fwrite(g_buf, 1, p, g_out)!=p)
    {
      perror("Fwrite() failed");
      exit(1);
    }
  }

  return 0;
}

int main(int argc, char** argv)
{

  int level=4;
  int i, dummy;

  char *in_name;

  bool do_decomp=false;
  bool overwrite=false;

  const int magic=LZ4_MAGIC;

  char out_name[FILENAME_MAX];

  while (argc>1 && *argv[1]=='-')
  {
    for (i=1; argv[1][i]!='\0'; ++i)
    {
      switch (argv[1][i])
      {
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        level=argv[1][i]-'0';
        break;
      case 'd':
        do_decomp=true;
        break;
      case 'f':
        overwrite=true;
        break;
      default:
        fprintf(stderr, "Unknown option: -%c\n", argv[1][i]);
        exit(1);
      }
    }

    --argc;
    ++argv;
  }

  if (argc<2)
  {
    fprintf(stderr,
        "LZ4 compressor/decompressor\n"
        "\n"
        "Usage: LZ4 [options] infile [outfile]\n"
        "\n"
        "Options:\n"
        "  -1  Compress faster\n"
        "  -9  Compress better\n"
        "  -d  Decompress\n"
        "  -f  Force overwrite of output file\n");
    exit(1);
  }

  in_name = argv[1];

  g_in=fopen(in_name, "rb");
  if (!g_in)
  {
    perror(in_name);
    exit(1);
  }

  if (argc<3)
  {
    strcpy(out_name, in_name);
    if (do_decomp)
    {
      const int p=strlen(out_name)-4;
      if (p>0 && strcmp(&out_name[p], ".lz4")==0)
        out_name[p]='\0';
      else
        strcat(out_name, ".out");
    }
    else
      strcat(out_name, ".lz4");
  }
  else
    strcpy(out_name, argv[2]);

  if (!overwrite)
  {
    FILE* f=fopen(out_name, "rb");
    if (f)
    {
      fclose(f);

      fprintf(stderr, "%s already exists. Overwrite (y/n)? ", out_name);
      fflush(stderr);

      if (getchar()!='y')
      {
        fprintf(stderr, "Not overwritten\n");
        exit(1);
      }
    }
  }

  g_buf=(U8*)malloc((BLOCK_SIZE+BLOCK_SIZE+EXCESS)*sizeof(U8));
  if (g_buf==NULL)
  {
    fprintf(stderr, "Not enough memory\n");
    exit(1);
  }

  if (do_decomp)
  {
    int magic;
    dummy = fread(&magic, 1, sizeof(magic), g_in);
    if (magic!=LZ4_MAGIC)
    {
      fprintf(stderr, "%s: Not in Legacy format\n", in_name);
      exit(1);
    }

    g_out=fopen(out_name, "wb");
    if (!g_out)
    {
      perror(out_name);
      exit(1);
    }

    fprintf(stderr, "Decompressing %s\n", in_name);

    if (decompress()!=0)
    {
      fprintf(stderr, "%s: Corrupt input\n", in_name);
      exit(1);
    }
  }
  else
  {
    g_out=fopen(out_name, "wb");
    if (!g_out)
    {
      perror(out_name);
      exit(1);
    }

    fwrite(&magic, 1, sizeof(magic), g_out);

    fprintf(stderr, "Compressing %s\n", in_name);

    compress((level<9)?1<<level:WINDOW_SIZE);  

  }

  fclose(g_in);
  fclose(g_out);

  return 0;
}
