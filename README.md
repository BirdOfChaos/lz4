# LZ4 compression / decompression

## Description
A simple lz4 library in C that can do compression and decompression. It is meant to be simple and easily portable to other platforms. Can compress files or memory arrays. Works in little endian and big endian environments.
### Note
2021-12-06: apparently the C port was already done years ago for [version 1.10 of LZ4X](https://github.com/nemequ/lz4x)

## TODO
    
**Create switch for older architecture**
- Probably a define at compile time
- For size optimization that will reduce memory requirements
  
**Optimize for speed on older architectures**
- Dependency: create switch for older architecture
  
**Test on other platforms**
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
