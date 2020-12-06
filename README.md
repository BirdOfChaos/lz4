# LZ4 compression / decompression

## Description
A simple lz4 library in C that can do compression and decompression. It is meant to be simple and easily portable to other platforms. Can compress files of memory arrays. Works in little endian and big endian environments.

## TODO
** ~~Finish porting to C~~ **  
- So that older architectures/compilers pre-dating C++ can generate the binary
- ** DONE **
  
** Separate into compress/decompress library **
- With a test main program
- Will allow for easy integration in other projects
  
** Create switch for older architecture **
- Probably a define at compile time
- For size optimization that will reduce memory requirements
  
** Optimize for speed on older architectures **
- Dependency: create switch for older architecture
  
** Test on other platforms **
- Currently supported: x86_64 Linux, m68k Atari ST
- Planned: m68k macintosh, m68k amiga, MIPS, SPARC
  
## License
The original very permissive "unlicense" has been kept. See [LICENSE](LICENSE)

## Author
Laurent Chardon

## Based on LZ4X v1.60 from Ilya Muravyov
LZ4X is an LZ4-compatible file compressor which uses optimized string parsing to provide higher compression ratios.  
[The original code from Ilya can be found here](https://github.com/encode84/lz4x)

## Thanks
Thanks to Ilya Muravyov for the original code.
