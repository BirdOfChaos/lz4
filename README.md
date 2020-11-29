# LZ4X compression for Atari ST


### Based on LZ4X v1.60 from Ilya Muravyov
[The original code fomr Ilya can be found here](https://github.com/encode84/lz4x)

### Description
LZ4X is an LZ4-compatible file compressor which uses optimized string parsing to provide higher compression ratios.  
The Atari ST version is scaled down and only the regular lz4 algorithm is used, with smaller tables.  
At the moment, only compression is working and has been tested.  
Future work includes adapting decompression to the ST, porting the project to C code that can be entirely compiled on the ST and testing on other platforms.

### License
The original very permissive "unlicense" has been kept. See [LICENSE](LICENSE)

### Author
Laurent Chardon

### Thanks
Thanks to Ilya Muravyov for the original code.
