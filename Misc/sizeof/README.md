# sizeof

Test to show the size in bytes of various values in a Spectrum program
written with Z88DK.

For easy reference, the values are:

```
  sizeof(char)         = 1
  sizeof(int)          = 2
  sizeof(long)         = 4
  sizeof(long long)    = 8
  sizeof(float)        = 4
  sizeof(double)       = ERR

  sizeof(void*)        = 2

  sizeof(size_t)       = 2
  sizeof(enum) (8bit)  = 1
  sizeof(enum) (16bit) = 2

  sizeof(uint8_t)      = 1
  sizeof(uint16_t)     = 2
  sizeof(uint32_t)     = 4
  sizeof(uint64_t)     = 8
```
